/*
 * memtest86 - Memory testing utility for deepin 25
 * Command line interface.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef MEMTEST_CLI_H
#define MEMTEST_CLI_H

#include <QCoreApplication>
#include <QStringList>

namespace memtest {

struct CliOptions {
    quint64 sizeBytes = 0;          // 0 => auto (half of available memory)
    int passes = 1;
    QString testSpec;               // comma separated test ids, empty => all
    int fadeDelayMs = -1;           // -1 => core default (1000 ms)
    bool selfTest = false;
    bool verbose = false;
    bool help = false;
    QString err;
};

// Parse command line; returns false on error (err set).
bool parseCliOptions(const QStringList &args, CliOptions *opts);
void printHelp();
int runCli(const CliOptions &opts);

} // namespace memtest

#endif // MEMTEST_CLI_H
