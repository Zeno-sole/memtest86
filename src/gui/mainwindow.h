/*
 * memtest86 - Memory testing utility for deepin 25
 * DTK GUI main window.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef MEMTEST_MAINWINDOW_H
#define MEMTEST_MAINWINDOW_H

#include <DMainWindow>

#include "../core/memtester.h"

DWIDGET_USE_NAMESPACE

class QLabel;
class QProgressBar;
class QPushButton;
class QSpinBox;
class QComboBox;
class QTableWidget;
class QGroupBox;
class QCheckBox;

namespace memtest {

class MemTesterCore;

class MainWindow : public DMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onStartStopClicked();
    void onProgress(const memtest::ProgressInfo &info);
    void onError(const memtest::MemError &err);
    void onTestStarted(int testId, int pass, int totalPasses);
    void onFinished(bool ok, int errorCount, quint64 bytesTested, quint64 durationMs);

private:
    void buildUi();
    void setupConnections();
    void updateState(bool running);
    quint64 selectedSizeBytes() const;

    MemTesterCore *m_tester = nullptr;

    // Control widgets
    QComboBox *m_sizeCombo = nullptr;
    QSpinBox *m_passSpin = nullptr;
    QCheckBox *m_testChecks[9] = {nullptr};
    QPushButton *m_startButton = nullptr;

    // Status widgets
    QLabel *m_statusLabel = nullptr;
    QLabel *m_testLabel = nullptr;
    QProgressBar *m_progress = nullptr;
    QTableWidget *m_errorTable = nullptr;
    QLabel *m_resultLabel = nullptr;

    bool m_running = false;
};

} // namespace memtest

#endif // MEMTEST_MAINWINDOW_H
