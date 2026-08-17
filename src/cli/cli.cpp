/*
 * memtest86 - Memory testing utility for deepin 25
 * Command line interface implementation.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "cli.h"
#include "../core/memtester.h"

#include <QTextStream>
#include <QTimer>
#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QRegularExpression>
#include <QLibraryInfo>
#include <cstdio>

namespace memtest {

static QTextStream &out()
{
    static QTextStream ts(stdout);
    return ts;
}

bool parseSize(const QString &s, quint64 *bytes)
{
    static const QRegularExpression re(QStringLiteral("^\\s*([0-9]+(?:\\.[0-9]+)?)\\s*([KMGkmg]?)\\s*$"));
    const QRegularExpressionMatch m = re.match(s);
    if (!m.hasMatch())
        return false;
    bool ok = false;
    const double v = m.captured(1).toDouble(&ok);
    if (!ok)
        return false;
    const QString suffix = m.captured(2).toUpper();
    quint64 mult = 1;
    if (suffix == QStringLiteral("K")) mult = 1024ULL;
    else if (suffix == QStringLiteral("M")) mult = 1024ULL * 1024;
    else if (suffix == QStringLiteral("G")) mult = 1024ULL * 1024 * 1024;
    *bytes = static_cast<quint64>(v * mult);
    return *bytes > 0;
}

bool parseCliOptions(const QStringList &args, CliOptions *opts)
{
    for (int i = 0; i < args.size(); ++i) {
        const QString a = args.at(i);
        if (a == "-h" || a == "--help") {
            opts->help = true;
        } else if (a == "-s" || a == "--size") {
            if (i + 1 >= args.size()) { opts->err = QStringLiteral("missing value for %1").arg(a); return false; }
            if (!parseSize(args.at(++i), &opts->sizeBytes)) {
                opts->err = QStringLiteral("invalid size: %1").arg(args.at(i));
                return false;
            }
        } else if (a == "-p" || a == "--passes") {
            if (i + 1 >= args.size()) { opts->err = QStringLiteral("missing value for %1").arg(a); return false; }
            bool ok = false;
            opts->passes = args.at(++i).toInt(&ok);
            if (!ok || opts->passes < 1) { opts->err = QStringLiteral("invalid passes: %1").arg(args.at(i)); return false; }
        } else if (a == "-t" || a == "--tests") {
            if (i + 1 >= args.size()) { opts->err = QStringLiteral("missing value for %1").arg(a); return false; }
            opts->testSpec = args.at(++i);
        } else if (a == "-v" || a == "--verbose") {
            opts->verbose = true;
        } else if (a == "--cli") {
            // accepted; mode selection handled by main
        } else {
            opts->err = QStringLiteral("unknown option: %1").arg(a);
            return false;
        }
    }
    return true;
}

void printHelp()
{
    out() << QObject::tr(
        "Usage: memtest86 [options]\n"
        "\n"
        "A memory diagnostic tool for deepin 25 (inspired by memtest86).\n"
        "\n"
        "Options:\n"
        "  --cli                 Run in command line mode (default is GUI)\n"
        "  -s, --size <size>     Memory size to test, e.g. 256M, 1G, 512K\n"
        "                        (default: half of available memory)\n"
        "  -p, --passes <n>      Number of test passes (default: 1)\n"
        "  -t, --tests <list>    Comma separated test ids, e.g. 0,2,4 (default: all)\n"
        "  -v, --verbose         Verbose output\n"
        "  -h, --help            Show this help\n"
        "\n"
        "Tests:\n"
        "  0  Address test, walking ones\n"
        "  1  Address test, own address\n"
        "  2  Moving inversions, ones & zeros\n"
        "  3  Moving inversions, 8-bit pattern\n"
        "  4  Moving inversions, random pattern\n"
        "  5  Block move, 64-bit moves\n"
        "  6  Moving inversions, 32-bit blocks\n"
        "  7  Random number sequence\n"
        "  9  Bit fade test, 2 patterns\n") << Qt::flush;
}

int runCli(const CliOptions &opts)
{
    QCoreApplication *app = QCoreApplication::instance();

    MemTesterCore tester;

    // Determine test region
    quint64 size = opts.sizeBytes;
    if (size == 0)
        size = MemTesterCore::availablePhysicalMemory() / 2;
    // Cap at 4 GiB default to keep the CLI snappy unless user asked for more.
    if (opts.sizeBytes == 0 && size > 4ULL * 1024 * 1024 * 1024)
        size = 4ULL * 1024 * 1024 * 1024;

    out() << QObject::tr("MemTest86 - memory diagnostic (deepin 25)\n");
    out() << QObject::tr("Testing %1 of memory, %2 pass(es)\n")
                 .arg(MemTesterCore::humanSize(size))
                 .arg(opts.passes);

    if (!tester.allocateBuffer(size)) {
        out() << QObject::tr("ERROR: failed to allocate %1 of memory\n").arg(MemTesterCore::humanSize(size));
        return 1;
    }

    QVector<int> tests;
    if (opts.testSpec.isEmpty()) {
        const auto all = MemTesterCore::allTests();
        for (const auto &t : all)
            tests.append(t.id);
    } else {
        const QStringList parts = opts.testSpec.split(',', Qt::SkipEmptyParts);
        for (const QString &p : parts) {
            bool ok = false;
            int id = p.trimmed().toInt(&ok);
            if (ok) tests.append(id);
        }
        if (tests.isEmpty()) {
            out() << QObject::tr("ERROR: no valid test ids in '%1'\n").arg(opts.testSpec);
            return 1;
        }
    }

    tester.setTests(tests);
    tester.setPassCount(opts.passes);
    tester.setVerbose(opts.verbose);

    // Progress printing
    QObject::connect(&tester, &MemTesterCore::testStarted, [](int testId, int pass, int total) {
        out() << QString("\n--- %1/%2 %3 ---\n")
                     .arg(pass + 1)
                     .arg(total)
                     .arg(MemTesterCore::testName(testId));
        out().flush();
    });
    QObject::connect(&tester, &MemTesterCore::errorDetected, [](const MemError &e) {
        out() << QObject::tr("  ERROR @ 0x%1: expected 0x%2, got 0x%3\n")
                     .arg(e.address, 0, 16)
                     .arg(e.expected, 0, 16)
                     .arg(e.actual, 0, 16);
        out().flush();
    });
    QObject::connect(&tester, &MemTesterCore::logMessage, [](const QString &m) {
        out() << "  " << m << "\n";
        out().flush();
    });

    bool done = false;
    QObject::connect(&tester, &MemTesterCore::finished,
                     [&done, &tester](bool ok, int errCount, quint64 bytes, quint64 ms) {
        out() << "\n";
        if (ok)
            out() << QObject::tr("RESULT: PASS - %1 tested, %2 errors, %3 s\n")
                         .arg(MemTesterCore::humanSize(bytes))
                         .arg(errCount)
                         .arg(QString::number(ms / 1000.0, 'f', 1));
        else
            out() << QObject::tr("RESULT: FAIL - %1 errors detected\n").arg(errCount);
        out().flush();
        done = true;
        QCoreApplication::quit();
        Q_UNUSED(tester);
    });

    tester.start();

    // Keep the event loop alive until finished signal quits it.
    while (!done)
        app->processEvents(QEventLoop::WaitForMoreEvents);

    return (done && tester.errorCount() == 0) ? 0 : 1;
}

} // namespace memtest
