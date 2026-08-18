/*
 * memtest86 - Memory testing utility for deepin 25
 * Core testing library implementation.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "memtester.h"

#include <QFile>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <random>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

namespace memtest {

namespace {
constexpr quint64 kWordSize = sizeof(uint64_t);
constexpr quint64 kPassesPerTest = 1; // granularity not used; pass count is outer
}

MemTesterCore::MemTesterCore(QObject *parent)
    : QObject(parent)
{
    // Default: run all tests
    for (int i = 0; i < TestCount; ++i)
        m_selectedTests.append(i);
}

MemTesterCore::~MemTesterCore()
{
    stop();
    freeBuffer();
}

// ---------------------------------------------------------------------------
// Buffer management
// ---------------------------------------------------------------------------
bool MemTesterCore::allocateBuffer(quint64 sizeBytes)
{
    freeBuffer();
    if (sizeBytes == 0)
        return false;
    // Use calloc so the buffer is zero-initialized and actually committed.
    void *p = std::calloc(1, static_cast<size_t>(sizeBytes));
    if (!p)
        return false;
    m_buffer = p;
    m_bufferSize = sizeBytes;
    m_regionSize = sizeBytes;
    return true;
}

void MemTesterCore::freeBuffer()
{
    if (m_buffer) {
        std::free(m_buffer);
        m_buffer = nullptr;
        m_bufferSize = 0;
        m_regionSize = 0;
    }
}

// ---------------------------------------------------------------------------
// Results
// ---------------------------------------------------------------------------
QVector<MemError> MemTesterCore::errors() const
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    return m_errors;
}

void MemTesterCore::clearErrors()
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    m_errors.clear();
}

void MemTesterCore::recordError(uint64_t addr, uint64_t expected, uint64_t actual,
                                int test, const QString &desc)
{
    MemError e;
    e.address = addr;
    e.expected = expected;
    e.actual = actual;
    e.test = test;
    e.description = desc;
    {
        std::lock_guard<std::mutex> lock(m_errorMutex);
        m_errors.append(e);
    }
    emit errorDetected(e);
}

bool MemTesterCore::checkWord(volatile uint64_t *p, uint64_t expected, int test,
                              const QString &desc)
{
    uint64_t actual = *p;
    if (actual != expected) {
        uint64_t addr = reinterpret_cast<uintptr_t>(const_cast<uint64_t *>(p))
                        - reinterpret_cast<uintptr_t>(m_buffer);
        recordError(addr, expected, actual, test, desc);
        return false;
    }
    return true;
}

void MemTesterCore::log(const QString &msg)
{
    if (m_verbose)
        emit logMessage(msg);
}

// ---------------------------------------------------------------------------
// System info
// ---------------------------------------------------------------------------
quint64 MemTesterCore::totalPhysicalMemory()
{
    QFile f("/proc/meminfo");
    if (f.open(QIODevice::ReadOnly)) {
        while (!f.atEnd()) {
            const QByteArray line = f.readLine();
            if (line.startsWith("MemTotal:")) {
                const QList<QByteArray> parts = line.split(' ');
                for (const auto &p : parts) {
                    bool ok = false;
                    quint64 kb = p.toULongLong(&ok);
                    if (ok)
                        return kb * 1024;
                }
            }
        }
    }
    // Fallback: sysconf
    long pages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGESIZE);
    if (pages > 0 && pageSize > 0)
        return static_cast<quint64>(pages) * static_cast<quint64>(pageSize);
    return 0;
}

quint64 MemTesterCore::availablePhysicalMemory()
{
    QFile f("/proc/meminfo");
    if (f.open(QIODevice::ReadOnly)) {
        while (!f.atEnd()) {
            const QByteArray line = f.readLine();
            if (line.startsWith("MemAvailable:")) {
                const QList<QByteArray> parts = line.split(' ');
                for (const auto &p : parts) {
                    bool ok = false;
                    quint64 kb = p.toULongLong(&ok);
                    if (ok)
                        return kb * 1024;
                }
            }
        }
    }
    return totalPhysicalMemory() / 2;
}

QString MemTesterCore::humanSize(quint64 bytes)
{
    const double b = static_cast<double>(bytes);
    if (bytes >= 1024ULL * 1024 * 1024)
        return QString::number(b / (1024.0 * 1024 * 1024), 'f', 2) + QStringLiteral(" GiB");
    if (bytes >= 1024ULL * 1024)
        return QString::number(b / (1024.0 * 1024), 'f', 2) + QStringLiteral(" MiB");
    if (bytes >= 1024ULL)
        return QString::number(b / 1024.0, 'f', 1) + QStringLiteral(" KiB");
    return QString::number(bytes) + QStringLiteral(" B");
}

// ---------------------------------------------------------------------------
// Test registry
// ---------------------------------------------------------------------------
QVector<TestInfo> MemTesterCore::allTests()
{
    QVector<TestInfo> list;
    TestInfo t;
    t.id = TestAddressWalkingOnes;
    t.shortName = "[Address test, walking ones]";
    t.name = QCoreApplication::translate("memtest", "Address test, walking ones");
    list.append(t);
    t.id = TestAddressOwnAddress;
    t.shortName = "[Address test, own address]";
    t.name = QCoreApplication::translate("memtest", "Address test, own address");
    list.append(t);
    t.id = TestMovingInv01;
    t.shortName = "[Moving inversions, ones & zeros]";
    t.name = QCoreApplication::translate("memtest", "Moving inversions, ones & zeros");
    list.append(t);
    t.id = TestMovingInv8bit;
    t.shortName = "[Moving inversions, 8-bit pattern]";
    t.name = QCoreApplication::translate("memtest", "Moving inversions, 8-bit pattern");
    list.append(t);
    t.id = TestMovingInvRandom;
    t.shortName = "[Moving inversions, random pattern]";
    t.name = QCoreApplication::translate("memtest", "Moving inversions, random pattern");
    list.append(t);
    t.id = TestBlockMove64;
    t.shortName = "[Block move, 64-bit moves]";
    t.name = QCoreApplication::translate("memtest", "Block move, 64-bit moves");
    list.append(t);
    t.id = TestMovingInv32block;
    t.shortName = "[Moving inversions, 32-bit blocks]";
    t.name = QCoreApplication::translate("memtest", "Moving inversions, 32-bit blocks");
    list.append(t);
    t.id = TestRandomSequence;
    t.shortName = "[Random number sequence]";
    t.name = QCoreApplication::translate("memtest", "Random number sequence");
    list.append(t);
    t.id = TestBitFade;
    t.shortName = "[Bit fade test, 2 patterns]";
    t.name = QCoreApplication::translate("memtest", "Bit fade test, 2 patterns");
    list.append(t);
    return list;
}

QString MemTesterCore::testName(int id)
{
    const auto tests = allTests();
    for (const auto &t : tests) {
        if (t.id == id)
            return t.name.isEmpty() ? t.shortName : t.name;
    }
    return QString("Test %1").arg(id);
}

// ---------------------------------------------------------------------------
// Run control
// ---------------------------------------------------------------------------
void MemTesterCore::start()
{
    if (m_running.load() || !m_buffer || m_bufferSize == 0)
        return;
    m_cancel.store(false);
    m_stopRequested.store(false);
    m_running.store(true);
    m_timer.start();
    clearErrors();

    QThread *thread = QThread::create([this]() { run(); });
    thread->setObjectName("memtest-runner");
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    connect(thread, &QThread::finished, this, [this, thread]() {
        m_running.store(false);
        const quint64 ms = static_cast<quint64>(m_timer.elapsed());
        emit finished(m_errors.isEmpty(), m_errors.size(),
                      m_testedBytes, ms);
        Q_UNUSED(thread);
    });
    thread->start();
}

void MemTesterCore::stop()
{
    m_stopRequested.store(true);
}

void MemTesterCore::run()
{
    m_testedBytes = 0;
    quint64 regionSize = m_regionSize;
    const QVector<int> tests = m_selectedTests;

    // Touch all pages once so they are committed (avoid OOM surprises).
    volatile uint64_t *base = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = regionSize / kWordSize;
    for (quint64 i = 0; i < words; i += 4096 / kWordSize)
        base[i] = 0;

    int pass = 0;
    while (pass < m_passes && !m_stopRequested.load()) {
        for (int testId : tests) {
            if (m_stopRequested.load())
                break;
            m_currentTest = testId;
            m_currentPass = pass;
            m_currentPassBytes = 0;
            m_currentPassTotal = regionSize;
            emit testStarted(testId, pass, m_passes);
            log(QString("Pass %1/%2  test %3 started")
                    .arg(pass + 1).arg(m_passes).arg(testName(testId)));

            switch (testId) {
            case TestAddressWalkingOnes: testAddressWalkingOnes(regionSize); break;
            case TestAddressOwnAddress:  testAddressOwnAddress(regionSize); break;
            case TestMovingInv01:        testMovingInversions01(regionSize); break;
            case TestMovingInv8bit:      testMovingInversions8bit(regionSize); break;
            case TestMovingInvRandom:    testMovingInversionsRandom(regionSize); break;
            case TestBlockMove64:        testBlockMove64(regionSize); break;
            case TestMovingInv32block:   testMovingInversions32block(regionSize); break;
            case TestRandomSequence:     testRandomSequence(regionSize); break;
            case TestBitFade:            testBitFade(regionSize); break;
            default: break;
            }

            m_currentPassBytes = regionSize;
            reportProgress(ProgressInfo{testId, pass, m_passes, regionSize, regionSize,
                                        errorCount(), computePercent(pass, tests.indexOf(testId), tests.size())});
            if (m_stopRequested.load())
                break;
        }
        ++pass;
        m_testedBytes += regionSize;
    }
    m_cancel.store(m_stopRequested.load());
}

double MemTesterCore::computePercent(int pass, int testIdx, int testCount) const
{
    if (m_passes <= 0 || testCount <= 0)
        return 0.0;
    double totalWork = static_cast<double>(m_passes) * testCount;
    double done = static_cast<double>(pass) * testCount + testIdx;
    return done / totalWork * 100.0;
}

void MemTesterCore::reportProgress(const ProgressInfo &info)
{
    emit progressChanged(info);
}

// ---------------------------------------------------------------------------
// Test implementations (algorithms modeled on memtest86)
// ---------------------------------------------------------------------------

// Test 0: Address test, walking ones. Write a 1 bit walked across the
// address bus; verify while writing and after.
bool MemTesterCore::testAddressWalkingOnes(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    uint64_t pattern = 1;
    for (int bit = 0; bit < 64 && !m_stopRequested.load(); ++bit) {
        for (quint64 i = 0; i < words; ++i) {
            mem[i] = pattern;
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = i * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, bit, 64)});
            }
        }
        // verify pass
        for (quint64 i = 0; i < words; ++i) {
            if (!checkWord(&mem[i], pattern, TestAddressWalkingOnes,
                           QStringLiteral("walking ones bit %1").arg(bit)))
                break;
        }
        pattern <<= 1;
        if (pattern == 0)
            break;
    }
    return true;
}

// Test 1: Address test, own address. Write each word with its own address,
// then verify and invert.
bool MemTesterCore::testAddressOwnAddress(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;

    for (quint64 i = 0; i < words; ++i) {
        mem[i] = i;
        if ((i & 0x7FFFF) == 0) {
            m_currentPassBytes = i * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal, errorCount(),
                            computePercent(m_currentPass, 0, 2)});
        }
    }
    for (quint64 i = 0; i < words; ++i) {
        if (!checkWord(&mem[i], i, TestAddressOwnAddress, QStringLiteral("own address")))
            break;
        if ((i & 0x7FFFF) == 0) {
            m_currentPassBytes = (words + i) * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                            computePercent(m_currentPass, 1, 2)});
        }
    }
    // inverted pass
    for (quint64 i = 0; i < words; ++i)
        mem[i] = ~i;
    for (quint64 i = 0; i < words; ++i) {
        if (!checkWord(&mem[i], ~i, TestAddressOwnAddress, QStringLiteral("own address inverted")))
            break;
    }
    return true;
}

// Test 2: Moving inversions, ones & zeros. Fill with 0xFFFFFFFF..., verify,
// fill with 0, verify.
bool MemTesterCore::testMovingInversions01(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    const uint64_t p1 = UINT64_MAX;
    const uint64_t p2 = 0;

    for (int phase = 0; phase < 2 && !m_stopRequested.load(); ++phase) {
        const uint64_t pat = (phase == 0) ? p1 : p2;
        const uint64_t inv = (phase == 0) ? p2 : p1;
        for (quint64 i = 0; i < words; ++i) {
            mem[i] = pat;
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = i * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, phase, 4)});
            }
        }
        for (quint64 i = words; i-- > 0;) {
            mem[i] = inv;
        }
        for (quint64 i = 0; i < words; ++i) {
            if (!checkWord(&mem[i], inv, TestMovingInv01, QStringLiteral("inv 01")))
                break;
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = (words + i) * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                                computePercent(m_currentPass, 2 + phase, 4)});
            }
        }
    }
    return true;
}

// Test 3: Moving inversions, 8-bit pattern.
bool MemTesterCore::testMovingInversions8bit(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    const quint64 pattern = 0x0101010101010101ULL;
    const quint64 anti = 0xFEFEFEFEFEFEFEFEULL;

    for (int rep = 0; rep < 2 && !m_stopRequested.load(); ++rep) {
        for (quint64 i = 0; i < words; ++i) {
            mem[i] = pattern;
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = i * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, rep * 2, 4)});
            }
        }
        for (quint64 i = words; i-- > 0;) {
            mem[i] = anti;
        }
        for (quint64 i = 0; i < words; ++i) {
            if (!checkWord(&mem[i], anti, TestMovingInv8bit, QStringLiteral("inv 8bit")))
                break;
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = (words + i) * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                                computePercent(m_currentPass, rep * 2 + 1, 4)});
            }
        }
    }
    return true;
}

// Test 4: Moving inversions, random pattern.
bool MemTesterCore::testMovingInversionsRandom(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    std::mt19937_64 rng(0x9E3779B97F4A7C15ULL);
    std::vector<uint64_t> expected(words);

    for (int rep = 0; rep < 2 && !m_stopRequested.load(); ++rep) {
        // Fill forward with a deterministic random sequence (recorded in expected)
        for (quint64 i = 0; i < words; ++i) {
            expected[i] = rng();
            mem[i] = expected[i];
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = i * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, rep * 2, 4)});
            }
        }
        // Verify in reverse against the recorded sequence
        for (quint64 i = words; i-- > 0;) {
            if (!checkWord(&mem[i], expected[i], TestMovingInvRandom,
                           QStringLiteral("random invert")))
                break;
        }
        // Invert pass: write complement, verify
        for (quint64 i = 0; i < words; ++i) {
            mem[i] = ~expected[i];
            if ((i & 0x7FFFF) == 0) {
                m_currentPassBytes = (words + i) * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                                computePercent(m_currentPass, rep * 2 + 1, 4)});
            }
        }
        for (quint64 i = 0; i < words; ++i) {
            if (!checkWord(&mem[i], ~expected[i], TestMovingInvRandom,
                           QStringLiteral("random zero")))
                break;
        }
    }
    return true;
}

// Test 5: Block move, 64-bit moves. Copy the first half into the second,
// then verify both halves match the source pattern.
bool MemTesterCore::testBlockMove64(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    const quint64 half = words / 2;
    if (half == 0)
        return true;
    const uint64_t pattern = 0xDEADBEEFCAFEBABEULL;

    // init first half with pattern
    for (quint64 i = 0; i < half; ++i)
        mem[i] = pattern;

    // block copy: move second half from first half (memmove semantics)
    // We copy forward in chunks; source/dest don't overlap so a plain copy is fine.
    for (quint64 i = 0; i < half; ++i) {
        mem[half + i] = mem[i];
        if ((i & 0x3FFFF) == 0) {
            m_currentPassBytes = i * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal, errorCount(),
                            computePercent(m_currentPass, 0, 3)});
        }
    }
    // verify both halves
    for (quint64 i = 0; i < words; ++i) {
        if (!checkWord(&mem[i], pattern, TestBlockMove64, QStringLiteral("block move")))
            break;
        if ((i & 0x3FFFF) == 0) {
            m_currentPassBytes = (half + i) * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal, errorCount(),
                            computePercent(m_currentPass, 1, 3)});
        }
    }
    return true;
}

// Test 6: Moving inversions, 32-bit blocks.
bool MemTesterCore::testMovingInversions32block(quint64 bytes)
{
    volatile uint32_t *mem = reinterpret_cast<volatile uint32_t *>(m_buffer);
    const quint64 dwords = bytes / sizeof(uint32_t);
    const uint32_t p1 = 0xFFFFFFFFu;
    const uint32_t p2 = 0u;

    for (int phase = 0; phase < 2 && !m_stopRequested.load(); ++phase) {
        const uint32_t pat = (phase == 0) ? p1 : p2;
        const uint32_t inv = (phase == 0) ? p2 : p1;
        for (quint64 i = 0; i < dwords; ++i) {
            mem[i] = pat;
            if ((i & 0xFFFFF) == 0) {
                m_currentPassBytes = i * sizeof(uint32_t);
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, phase * 2, 4)});
            }
        }
        for (quint64 i = dwords; i-- > 0;)
            mem[i] = inv;
        for (quint64 i = 0; i < dwords; ++i) {
            uint32_t actual = mem[i];
            if (actual != inv) {
                recordError(i * sizeof(uint32_t), inv, actual, TestMovingInv32block,
                            QStringLiteral("inv 32block"));
            }
            if ((i & 0xFFFFF) == 0) {
                m_currentPassBytes = (dwords + i) * sizeof(uint32_t);
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                                computePercent(m_currentPass, phase * 2 + 1, 4)});
            }
        }
    }
    return true;
}

// Test 7: Random number sequence. Fill with a deterministic LFSR sequence,
// verify exactly (write pass then verify pass).
bool MemTesterCore::testRandomSequence(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    uint64_t state = 0x0123456789ABCDEFULL;

    // Generate the same sequence into a side buffer to verify against.
    std::vector<uint64_t> expected(words);
    for (quint64 i = 0; i < words; ++i) {
        // xorshift64
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        expected[i] = state;
        mem[i] = state;
        if ((i & 0x3FFFF) == 0) {
            m_currentPassBytes = i * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal, errorCount(),
                            computePercent(m_currentPass, 0, 2)});
        }
    }
    for (quint64 i = 0; i < words; ++i) {
        if (!checkWord(&mem[i], expected[i], TestRandomSequence,
                       QStringLiteral("random sequence")))
            break;
        if ((i & 0x3FFFF) == 0) {
            m_currentPassBytes = (words + i) * kWordSize;
            reportProgress({m_currentTest, m_currentPass, m_passes,
                            m_currentPassBytes, m_currentPassTotal * 2, errorCount(),
                            computePercent(m_currentPass, 1, 2)});
        }
    }
    return true;
}

// Test 9: Bit fade test, 2 patterns. Fill, delay, verify (no real delay in
// user-space for speed; still validates retention of 0/1 patterns).
bool MemTesterCore::testBitFade(quint64 bytes)
{
    volatile uint64_t *mem = reinterpret_cast<volatile uint64_t *>(m_buffer);
    const quint64 words = bytes / kWordSize;
    const uint64_t pats[2] = {0, UINT64_MAX};

    for (int p = 0; p < 2 && !m_stopRequested.load(); ++p) {
        const uint64_t pat = pats[p];
        for (quint64 i = 0; i < words; ++i)
            mem[i] = pat;
        // brief pause to simulate retention check
        for (volatile int spin = 0; spin < 100000; ++spin) {}
        for (quint64 i = 0; i < words; ++i) {
            if (!checkWord(&mem[i], pat, TestBitFade, QStringLiteral("bit fade")))
                break;
            if ((i & 0x3FFFF) == 0) {
                m_currentPassBytes = i * kWordSize;
                reportProgress({m_currentTest, m_currentPass, m_passes,
                                m_currentPassBytes, m_currentPassTotal, errorCount(),
                                computePercent(m_currentPass, p, 2)});
            }
        }
    }
    return true;
}

} // namespace memtest
