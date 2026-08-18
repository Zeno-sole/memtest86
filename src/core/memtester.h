/*
 * memtest86 - Memory testing utility for deepin 25
 *
 * Core testing library, algorithms inspired by memtest86:
 *   Test 0: Address test, walking ones
 *   Test 1: Address test, own address
 *   Test 2: Moving inversions, ones & zeros
 *   Test 3: Moving inversions, 8-bit pattern
 *   Test 4: Moving inversions, random pattern
 *   Test 5: Block move, 64-bit moves
 *   Test 6: Moving inversions, 32-bit blocks
 *   Test 7: Random number sequence
 *   Test 9: Bit fade test, 2 patterns
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef MEMTEST_CORE_H
#define MEMTEST_CORE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QElapsedTimer>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <vector>

namespace memtest {

// Result of a single error detection
struct MemError {
    uint64_t address = 0;     // byte address
    uint64_t expected = 0;
    uint64_t actual = 0;
    int test = 0;             // test number
    QString description;
};

// Test identifiers
enum TestId {
    TestAddressWalkingOnes = 0,
    TestAddressOwnAddress  = 1,
    TestMovingInv01        = 2,
    TestMovingInv8bit      = 3,
    TestMovingInvRandom    = 4,
    TestBlockMove64        = 5,
    TestMovingInv32block   = 6,
    TestRandomSequence     = 7,
    TestBitFade            = 9,
    TestCount              = 9
};

struct TestInfo {
    int id = 0;
    QString name;      // translated name
    QString shortName; // e.g. "[Address test, walking ones]"
};

// Per-test progress callback data
struct ProgressInfo {
    int testId = 0;
    int pass = 0;          // current pass (0-based)
    int totalPasses = 0;
    quint64 testedBytes = 0;   // bytes completed in current pass
    quint64 totalBytes = 0;    // bytes to test per pass
    int errorCount = 0;
    double percent = 0.0;  // overall percent
};

class MemTesterCore : public QObject
{
    Q_OBJECT
public:
    explicit MemTesterCore(QObject *parent = nullptr);
    ~MemTesterCore() override;

    // Configuration
    void setTestRegion(quint64 startAddr, quint64 size);
    void setPassCount(int passes) { m_passes = passes; }
    void setTests(const QVector<int> &tests) { m_selectedTests = tests; }
    void setVerbose(bool v) { m_verbose = v; }
    // Delay (ms) between write and verify in the bit fade test. Real
    // memtest86 idles for minutes to let DRAM charge decay; default 1000 ms.
    void setBitFadeDelayMs(int ms) { m_bitFadeDelayMs = qMax(0, ms); }
    int bitFadeDelayMs() const { return m_bitFadeDelayMs; }

    // Buffer management: allocate/free test buffer
    bool allocateBuffer(quint64 sizeBytes);
    void freeBuffer();
    quint64 bufferSize() const { return m_bufferSize; }
    void *buffer() const { return m_buffer; }

    // Run control
    void start();
    void stop();
    bool isRunning() const { return m_running.load(); }
    bool isCancelled() const { return m_cancel.load(); }

    // Results
    int errorCount() const { return m_errors.size(); }
    QVector<MemError> errors() const;
    void clearErrors();

    // System helpers
    static quint64 totalPhysicalMemory();
    static quint64 availablePhysicalMemory();

    // Self-test: verify the detection logic itself works by injecting a
    // known corruption into a scratch buffer and checking that the test
    // algorithms catch it. Returns true when the injected fault was found.
    // Uses and replaces the current test buffer; call before a real run.
    bool selfTest(QString *detail = nullptr);

    // Format bytes as human readable string (e.g. "512 MiB")
    static QString humanSize(quint64 bytes);

    // Translate test ids into info
    static QVector<TestInfo> allTests();
    static QString testName(int id);

signals:
    void progressChanged(const memtest::ProgressInfo &info);
    void errorDetected(const memtest::MemError &err);
    void testStarted(int testId, int pass, int totalPasses);
    void finished(bool ok, int errorCount, quint64 bytesTested, quint64 durationMs);
    void logMessage(const QString &msg);

private:
    void run();
    void reportProgress(const ProgressInfo &info);
    double computePercent(int pass, int testIdx, int testCount) const;

    // Individual tests, operate on m_buffer
    bool testAddressWalkingOnes(quint64 bytes);
    bool testAddressOwnAddress(quint64 bytes);
    bool testMovingInversions01(quint64 bytes);
    bool testMovingInversions8bit(quint64 bytes);
    bool testMovingInversionsRandom(quint64 bytes);
    bool testBlockMove64(quint64 bytes);
    bool testMovingInversions32block(quint64 bytes);
    bool testRandomSequence(quint64 bytes);
    bool testBitFade(quint64 bytes);

    void recordError(uint64_t addr, uint64_t expected, uint64_t actual, int test,
                     const QString &desc);

    bool checkWord(volatile uint64_t *p, uint64_t expected, int test, const QString &desc);
    bool checkDWord(volatile uint32_t *p, uint32_t expected, int test, const QString &desc);

    void log(const QString &msg);

private:
    void *m_buffer = nullptr;
    quint64 m_bufferSize = 0;
    quint64 m_regionStart = 0;   // not used directly; buffer is the region
    quint64 m_regionSize = 0;

    int m_passes = 1;
    int m_bitFadeDelayMs = 1000;
    QVector<int> m_selectedTests;
    bool m_verbose = false;

    // current run state (accessed from worker thread)
    int m_currentTest = 0;
    int m_currentPass = 0;
    quint64 m_currentPassBytes = 0;
    quint64 m_currentPassTotal = 0;
    quint64 m_testedBytes = 0;

    std::atomic<bool> m_running{false};
    std::atomic<bool> m_cancel{false};
    std::atomic<bool> m_stopRequested{false};

    QVector<MemError> m_errors;
    mutable std::mutex m_errorMutex;
    QElapsedTimer m_timer;
};

} // namespace memtest

#endif // MEMTEST_CORE_H
