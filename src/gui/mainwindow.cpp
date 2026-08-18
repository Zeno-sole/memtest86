/*
 * memtest86 - Memory testing utility for deepin 25
 * DTK GUI main window implementation.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "mainwindow.h"
#include "../core/memtester.h"

#include <DFontSizeManager>
#include <DLabel>
#include <DProgressBar>
#include <DPushButton>
#include <DCheckBox>
#include <DComboBox>
#include <DSpinBox>
#include <DGroupBox>
#include <DTableWidget>
#include <DTitlebar>
#include <DSuggestButton>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QIcon>

DWIDGET_USE_NAMESPACE

namespace memtest {

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
    , m_tester(new MemTesterCore(this))
{
    setWindowTitle(tr("Memory Test"));
    setWindowIcon(QIcon::fromTheme(QStringLiteral("memtest86"),
                                   QIcon(QStringLiteral(":/icons/memtest86.svg"))));
    resize(860, 620);

    buildUi();
    setupConnections();
    updateState(false);
}

MainWindow::~MainWindow() = default;

quint64 MainWindow::selectedSizeBytes() const
{
    const int idx = m_sizeCombo->currentIndex();
    const quint64 avail = MemTesterCore::availablePhysicalMemory();
    switch (idx) {
    case 0: return avail / 2;                    // half
    case 1: return avail;                        // all available
    default: {
        // 256M / 512M / 1G / 2G / 4G
        const quint64 vals[] = {256ULL * 1024 * 1024, 512ULL * 1024 * 1024,
                                1ULL * 1024 * 1024 * 1024, 2ULL * 1024 * 1024 * 1024,
                                4ULL * 1024 * 1024 * 1024};
        const int v = idx - 2;
        if (v >= 0 && v < 5)
            return vals[v];
        return avail / 2;
    }
    }
}

void MainWindow::buildUi()
{
    DTitlebar *titlebar = this->titlebar();
    titlebar->setTitle(tr("Memory Test"));
    titlebar->setIcon(QIcon::fromTheme(QStringLiteral("diagnostics")));

    QWidget *central = new QWidget;
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(20, 16, 20, 16);
    root->setSpacing(12);

    // ---------- Configuration group ----------
    auto *cfgBox = new DGroupBox(tr("Test Configuration"));
    auto *cfgLayout = new QGridLayout(cfgBox);
    cfgLayout->setSpacing(10);

    // Size
    cfgLayout->addWidget(new QLabel(tr("Memory size:")), 0, 0);
    m_sizeCombo = new DComboBox;
    m_sizeCombo->addItem(tr("Half of available memory"));
    m_sizeCombo->addItem(tr("All available memory"));
    m_sizeCombo->addItem(QStringLiteral("256 MiB"));
    m_sizeCombo->addItem(QStringLiteral("512 MiB"));
    m_sizeCombo->addItem(QStringLiteral("1 GiB"));
    m_sizeCombo->addItem(QStringLiteral("2 GiB"));
    m_sizeCombo->addItem(QStringLiteral("4 GiB"));
    cfgLayout->addWidget(m_sizeCombo, 0, 1);

    // Passes
    cfgLayout->addWidget(new QLabel(tr("Passes:")), 0, 2);
    m_passSpin = new DSpinBox;
    m_passSpin->setRange(1, 99);
    m_passSpin->setValue(1);
    cfgLayout->addWidget(m_passSpin, 0, 3);

    // Tests
    cfgLayout->addWidget(new QLabel(tr("Tests:")), 1, 0, Qt::AlignTop);
    auto *testsWidget = new QWidget;
    auto *testsLayout = new QVBoxLayout(testsWidget);
    testsLayout->setContentsMargins(0, 0, 0, 0);
    testsLayout->setSpacing(4);
    const auto tests = MemTesterCore::allTests();
    for (int i = 0; i < tests.size(); ++i) {
        m_testChecks[i] = new DCheckBox(tests[i].name);
        m_testChecks[i]->setChecked(true);
        testsLayout->addWidget(m_testChecks[i]);
    }
    cfgLayout->addWidget(testsWidget, 1, 1, 1, 3);

    root->addWidget(cfgBox);

    // ---------- Start / stop ----------
    m_startButton = new DSuggestButton(tr("Start Test"));
    m_startButton->setMinimumHeight(36);
    m_startButton->setMinimumWidth(140);
    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(m_startButton);
    btnRow->addStretch();
    root->addLayout(btnRow);

    // ---------- Progress / status ----------
    auto *statusBox = new DGroupBox(tr("Status"));
    auto *statusLayout = new QVBoxLayout(statusBox);
    statusLayout->setSpacing(8);

    m_testLabel = new QLabel(tr("Ready"));
    DFontSizeManager::instance()->bind(m_testLabel, DFontSizeManager::T6);
    statusLayout->addWidget(m_testLabel);

    m_progress = new DProgressBar;
    m_progress->setRange(0, 100);
    m_progress->setValue(0);
    m_progress->setTextVisible(true);
    statusLayout->addWidget(m_progress);

    m_statusLabel = new QLabel;
    DFontSizeManager::instance()->bind(m_statusLabel, DFontSizeManager::T8);
    statusLayout->addWidget(m_statusLabel);

    root->addWidget(statusBox);

    // ---------- Errors ----------
    auto *errBox = new DGroupBox(tr("Errors"));
    auto *errLayout = new QVBoxLayout(errBox);
    errLayout->setContentsMargins(8, 8, 8, 8);
    m_errorTable = new DTableWidget(0, 4);
    m_errorTable->setHorizontalHeaderLabels({
        tr("Address"), tr("Expected"), tr("Actual"), tr("Test")
    });
    m_errorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_errorTable->verticalHeader()->setVisible(false);
    m_errorTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_errorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_errorTable->setAlternatingRowColors(true);
    errLayout->addWidget(m_errorTable);
    root->addWidget(errBox, 1);

    m_resultLabel = new QLabel;
    DFontSizeManager::instance()->bind(m_resultLabel, DFontSizeManager::T7);
    root->addWidget(m_resultLabel);

    setCentralWidget(central);
}

void MainWindow::setupConnections()
{
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(m_tester, &MemTesterCore::progressChanged, this, &MainWindow::onProgress);
    connect(m_tester, &MemTesterCore::errorDetected, this, &MainWindow::onError);
    connect(m_tester, &MemTesterCore::testStarted, this, &MainWindow::onTestStarted);
    connect(m_tester, &MemTesterCore::finished, this, &MainWindow::onFinished);
}

void MainWindow::updateState(bool running)
{
    m_running = running;
    m_sizeCombo->setEnabled(!running);
    m_passSpin->setEnabled(!running);
    for (int i = 0; i < 9; ++i) {
        if (m_testChecks[i])
            m_testChecks[i]->setEnabled(!running);
    }
    m_startButton->setText(running ? tr("Stop") : tr("Start Test"));
    m_startButton->setProperty("_d_btn_type", running ? "warning" : "suggest");
}

void MainWindow::onStartStopClicked()
{
    if (m_running) {
        m_tester->stop();
        m_statusLabel->setText(tr("Stopping..."));
        return;
    }

    // Collect selected tests
    QVector<int> tests;
    const auto all = MemTesterCore::allTests();
    for (int i = 0; i < all.size(); ++i) {
        if (m_testChecks[i] && m_testChecks[i]->isChecked())
            tests.append(all[i].id);
    }
    if (tests.isEmpty()) {
        m_statusLabel->setText(tr("Please select at least one test."));
        return;
    }

    const quint64 size = selectedSizeBytes();
    if (!m_tester->allocateBuffer(size)) {
        m_statusLabel->setText(tr("Failed to allocate %1 of memory.").arg(MemTesterCore::humanSize(size)));
        return;
    }

    m_errorTable->setRowCount(0);
    m_resultLabel->clear();
    m_progress->setValue(0);
    m_testLabel->setText(tr("Testing %1 ...").arg(MemTesterCore::humanSize(size)));

    m_tester->setTests(tests);
    m_tester->setPassCount(m_passSpin->value());
    m_tester->setVerbose(false);

    updateState(true);
    m_tester->start();
}

void MainWindow::onTestStarted(int testId, int pass, int totalPasses)
{
    m_testLabel->setText(tr("Pass %1/%2 - %3")
                             .arg(pass + 1)
                             .arg(totalPasses)
                             .arg(MemTesterCore::testName(testId)));
}

void MainWindow::onProgress(const memtest::ProgressInfo &info)
{
    m_progress->setValue(static_cast<int>(info.percent));
    m_statusLabel->setText(tr("Tested %1 / %2, %3 errors")
                               .arg(MemTesterCore::humanSize(info.testedBytes))
                               .arg(MemTesterCore::humanSize(info.totalBytes))
                               .arg(info.errorCount));
}

void MainWindow::onError(const memtest::MemError &err)
{
    const int row = m_errorTable->rowCount();
    m_errorTable->insertRow(row);

    auto *addr = new QTableWidgetItem(QStringLiteral("0x%1").arg(err.address, 0, 16));
    auto *exp = new QTableWidgetItem(QStringLiteral("0x%1").arg(err.expected, 0, 16));
    auto *act = new QTableWidgetItem(QStringLiteral("0x%1").arg(err.actual, 0, 16));
    auto *test = new QTableWidgetItem(QString::number(err.test));

    addr->setForeground(QColor(0xE6, 0x6A, 0x5C));
    m_errorTable->setItem(row, 0, addr);
    m_errorTable->setItem(row, 1, exp);
    m_errorTable->setItem(row, 2, act);
    m_errorTable->setItem(row, 3, test);
}

void MainWindow::onFinished(bool ok, int errorCount, quint64 bytesTested, quint64 durationMs)
{
    updateState(false);
    if (ok) {
        m_resultLabel->setText(tr("PASS - %1 tested in %2 s, no errors found.")
                                   .arg(MemTesterCore::humanSize(bytesTested))
                                   .arg(QString::number(durationMs / 1000.0, 'f', 1)));
    } else {
        m_resultLabel->setText(tr("FAIL - %1 errors detected.").arg(errorCount));
    }
    m_progress->setValue(100);
}

} // namespace memtest
