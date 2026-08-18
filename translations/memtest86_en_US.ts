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
        <location filename="../src/main.cpp" line="97"/>
        <source>Error: %1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="104"/>
        <location filename="../src/main.cpp" line="108"/>
        <source>Memory Test</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>memtest</name>
    <message>
        <location filename="../src/core/memtester.cpp" line="154"/>
        <source>failed to allocate scratch buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="165"/>
        <source>baseline read-back check failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="176"/>
        <source>fault injection was not detected (caught=%1, errors=%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="190"/>
        <source>test algorithm reported %1 errors on a clean buffer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="268"/>
        <source>Address test, walking ones</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="272"/>
        <source>Address test, own address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="276"/>
        <source>Moving inversions, ones &amp; zeros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="280"/>
        <source>Moving inversions, 8-bit pattern</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="284"/>
        <source>Moving inversions, random pattern</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="288"/>
        <source>Block move, 64-bit moves</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="292"/>
        <source>Moving inversions, 32-bit blocks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="296"/>
        <source>Random number sequence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/core/memtester.cpp" line="300"/>
        <source>Bit fade test, 2 patterns</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>memtest::MainWindow</name>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="47"/>
        <location filename="../src/gui/mainwindow.cpp" line="82"/>
        <source>Memory Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="91"/>
        <source>Test Configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="96"/>
        <source>Memory size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="98"/>
        <source>Half of available memory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="99"/>
        <source>All available memory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="108"/>
        <source>Passes:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="115"/>
        <source>Tests:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="131"/>
        <location filename="../src/gui/mainwindow.cpp" line="202"/>
        <source>Start Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="141"/>
        <source>Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="145"/>
        <source>Ready</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="162"/>
        <source>Errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="167"/>
        <source>Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="167"/>
        <source>Expected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="167"/>
        <source>Actual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="167"/>
        <source>Test</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="202"/>
        <source>Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="210"/>
        <source>Stopping...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="222"/>
        <source>Please select at least one test.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="228"/>
        <source>Failed to allocate %1 of memory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="235"/>
        <source>Testing %1 ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="247"/>
        <source>Pass %1/%2 - %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="256"/>
        <source>Tested %1 / %2, %3 errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="283"/>
        <source>PASS - %1 tested in %2 s, no errors found.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/gui/mainwindow.cpp" line="287"/>
        <source>FAIL - %1 errors detected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
