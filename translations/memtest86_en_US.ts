<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/cli/cli.cpp" line="87"/>
        <source>Usage: memtest86 [options]

A memory diagnostic tool for deepin 25 (inspired by memtest86).

Options:
  --cli                 Run in command line mode (default is GUI)
  -s, --size &lt;size&gt;     Memory size to test, e.g. 256M, 1G, 512K
                        (default: half of available memory)
  -p, --passes &lt;n&gt;      Number of test passes (default: 1)
  -t, --tests &lt;list&gt;    Comma separated test ids, e.g. 0,2,4 (default: all)
      --fade-delay &lt;ms&gt; Bit fade idle time between write and verify
                        (default: 1000 ms; real memtest86 idles minutes)
      --self-test       Inject a known fault and verify detection works
  -v, --verbose         Verbose output
  -h, --help            Show this help

Tests:
  0  Address test, walking ones
  1  Address test, own address
  2  Moving inversions, ones &amp; zeros
  3  Moving inversions, 8-bit pattern
  4  Moving inversions, random pattern
  5  Block move, 64-bit moves
  6  Moving inversions, 32-bit blocks
  7  Random number sequence
  9  Bit fade test, 2 patterns
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="124"/>
        <source>Self-test: injecting a known fault into a 16 KiB scratch buffer...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="129"/>
        <source>Self-test PASSED: fault injection was detected and a clean buffer
reported no errors (detection logic works).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="133"/>
        <source>Self-test FAILED: %1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="145"/>
        <source>MemTest86 - memory diagnostic (deepin 25)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="146"/>
        <source>Testing %1 of memory, %2 pass(es)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="151"/>
        <source>ERROR: failed to allocate %1 of memory
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="168"/>
        <source>ERROR: no valid test ids in &apos;%1&apos;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="188"/>
        <source>  ERROR @ 0x%1: expected 0x%2, got 0x%3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="204"/>
        <source>RESULT: PASS - %1 tested, %2 errors, %3 s
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/cli/cli.cpp" line="209"/>
        <source>RESULT: FAIL - %1 errors detected
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="99"/>
        <source>Error: %1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="106"/>
        <location filename="../src/main.cpp" line="110"/>
        <source>Memory Test</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>memtest</name>
    <message>
        <location filename="../src/core/memtester.cpp" line="157"/>
        <source>failed to allocate scratch buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="168"/>
        <source>baseline read-back check failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="179"/>
        <source>fault injection was not detected (caught=%1, errors=%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="193"/>
        <source>test algorithm reported %1 errors on a clean buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="271"/>
        <source>Address test, walking ones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="275"/>
        <source>Address test, own address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="279"/>
        <source>Moving inversions, ones &amp; zeros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="283"/>
        <source>Moving inversions, 8-bit pattern</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="287"/>
        <source>Moving inversions, random pattern</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="291"/>
        <source>Block move, 64-bit moves</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="295"/>
        <source>Moving inversions, 32-bit blocks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="299"/>
        <source>Random number sequence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="303"/>
        <source>Bit fade test, 2 patterns</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>memtest::MainWindow</name>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="49"/>
        <location filename="../src/gui/mainwindow.cpp" line="96"/>
        <source>Memory Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="66"/>
        <source>System: %1 total, %2 available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="106"/>
        <source>Test Configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="111"/>
        <source>Memory size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="113"/>
        <source>Half of available memory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="114"/>
        <source>All available memory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="123"/>
        <source>Passes:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="135"/>
        <source>Tests:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="147"/>
        <source>Select all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="148"/>
        <source>Clear all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="168"/>
        <source>Start Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="184"/>
        <source>Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="188"/>
        <source>Ready</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="205"/>
        <source>Errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="210"/>
        <source>Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="210"/>
        <source>Expected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="210"/>
        <source>Actual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="210"/>
        <source>Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="172"/>
        <source>Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="260"/>
        <source>Stopping...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="272"/>
        <source>Please select at least one test.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="278"/>
        <source>Failed to allocate %1 of memory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="285"/>
        <source>Testing %1 ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="297"/>
        <source>Pass %1/%2 - %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="306"/>
        <source>Tested %1 / %2, %3 errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="334"/>
        <source>Stopped - %1 tested in %2 s, %3 errors so far.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="338"/>
        <source>Stopped.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="340"/>
        <source>PASS - %1 tested in %2 s, no errors found.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="344"/>
        <source>FAIL - %1 errors detected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
