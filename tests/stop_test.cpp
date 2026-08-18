/* Manual core stop-responsiveness test (no GUI needed). */
#include "../src/core/memtester.h"
#include <QCoreApplication>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <cstdio>

using namespace memtest;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    MemTesterCore tester;

    quint64 reportedBytes = 0;
    bool reported = false;

    QObject::connect(&tester, &MemTesterCore::finished,
                     [&](bool ok, int errCount, quint64 bytes, quint64 ms) {
        reported = true;
        reportedBytes = bytes;
        qInfo("finished: ok=%d errors=%d bytes=%llu ms=%llu",
              ok, errCount, (unsigned long long)bytes, (unsigned long long)ms);
        QCoreApplication::quit();
    });
    QObject::connect(&tester, &MemTesterCore::testStarted,
                     [&](int id, int pass, int total) {
        qInfo("testStarted id=%d pass=%d/%d", id, pass + 1, total);
    });

    QVector<int> tests = {0, 1, 2, 3, 4, 5, 6, 7, 9};
    tester.setTests(tests);
    tester.setPassCount(1);

    const quint64 size = 256ULL * 1024 * 1024;
    if (!tester.allocateBuffer(size)) {
        qWarning("allocate failed");
        return 2;
    }

    QTimer::singleShot(200, [&]() {
        qInfo(">>> stopping after 200ms");
        tester.stop();
    });
    QTimer::singleShot(15000, [&]() {
        qInfo("TIMEOUT - test still running");
        QCoreApplication::exit(3);
    });

    tester.start();
    int rc = app.exec();

    if (!reported)
        qWarning("finished never emitted");
    else if (reportedBytes == 0)
        qWarning("BUG: reportedBytes == 0 despite test having run");

    qInfo("RESULT: reportedBytes=%llu (%s)",
          (unsigned long long)reportedBytes,
          reportedBytes > 0 ? "OK" : "BAD");
    return rc == 0 && reported && reportedBytes > 0 ? 0 : 1;
}
