<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>QObject</name>
    <message>
        <source>Usage: memtest86 [options]

A memory diagnostic tool for deepin 25 (inspired by memtest86).

Options:
  --cli                 Run in command line mode (default is GUI)
  -s, --size &lt;size&gt;     Memory size to test, e.g. 256M, 1G, 512K
                        (default: half of available memory)
  -p, --passes &lt;n&gt;      Number of test passes (default: 1)
  -t, --tests &lt;list&gt;    Comma separated test ids, e.g. 0,2,4 (default: all)
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
        <translation>用法：memtest86 [选项]

一个面向 deepin 25 的内存诊断工具（灵感来自 memtest86）。

选项：
  --cli                 以命令行模式运行（默认为图形界面）
  -s, --size &lt;大小&gt;     待测试内存大小，例如 256M、1G、512K
                        （默认：可用内存的一半）
  -p, --passes &lt;n&gt;      测试轮数（默认：1）
  -t, --tests &lt;列表&gt;     逗号分隔的测试编号，例如 0,2,4（默认：全部）
  -v, --verbose         详细输出
  -h, --help            显示本帮助

测试项目：
  0  地址测试，走查 1 位
  1  地址测试，自身地址
  2  移动反转，全 1 与全 0
  3  移动反转，8 位模式
  4  移动反转，随机模式
  5  块移动，64 位移动
  6  移动反转，32 位块
  7  随机数序列
  9  位衰减测试，2 种模式
</translation>
    </message>
    <message>
        <source>MemTest86 - memory diagnostic (deepin 25)
</source>
        <translation>MemTest86 - 内存诊断（deepin 25）
</translation>
    </message>
    <message>
        <source>Testing %1 of memory, %2 pass(es)
</source>
        <translation>正在测试 %1 内存，共 %2 轮
</translation>
    </message>
    <message>
        <source>ERROR: failed to allocate %1 of memory
</source>
        <translation>错误：无法分配 %1 内存
</translation>
    </message>
    <message>
        <source>ERROR: no valid test ids in &apos;%1&apos;
</source>
        <translation>错误：&apos;%1&apos; 中没有有效的测试编号</translation>
    </message>
    <message>
        <source>  ERROR @ 0x%1: expected 0x%2, got 0x%3
</source>
        <translation>错误 @ 0x%1：期望 0x%2，实际 0x%3</translation>
    </message>
    <message>
        <source>RESULT: PASS - %1 tested, %2 errors, %3 s
</source>
        <translation>结果：通过 - 已测试 %1，%2 个错误，用时 %3 秒
</translation>
    </message>
    <message>
        <source>RESULT: FAIL - %1 errors detected
</source>
        <translation>结果：失败 - 检测到 %1 个错误
</translation>
    </message>
    <message>
        <source>Error: %1
</source>
        <translation>错误：%1</translation>
    </message>
    <message>
        <source>Memory Test</source>
        <translation>内存测试</translation>
    </message>
</context>
<context>
    <name>memtest</name>
    <message>
        <source>Address test, walking ones</source>
        <translation>地址测试，走查 1 位</translation>
    </message>
    <message>
        <source>Address test, own address</source>
        <translation>地址测试，自身地址</translation>
    </message>
    <message>
        <source>Moving inversions, ones &amp; zeros</source>
        <translation>移动反转，全 1 与全 0</translation>
    </message>
    <message>
        <source>Moving inversions, 8-bit pattern</source>
        <translation>移动反转，8 位模式</translation>
    </message>
    <message>
        <source>Moving inversions, random pattern</source>
        <translation>移动反转，随机模式</translation>
    </message>
    <message>
        <source>Block move, 64-bit moves</source>
        <translation>块移动，64 位移动</translation>
    </message>
    <message>
        <source>Moving inversions, 32-bit blocks</source>
        <translation>移动反转，32 位块</translation>
    </message>
    <message>
        <source>Random number sequence</source>
        <translation>随机数序列</translation>
    </message>
    <message>
        <source>Bit fade test, 2 patterns</source>
        <translation>位衰减测试，2 种模式</translation>
    </message>
</context>
<context>
    <name>memtest::MainWindow</name>
    <message>
        <source>Memory Test</source>
        <translation>内存测试</translation>
    </message>
    <message>
        <source>Test Configuration</source>
        <translation>测试配置</translation>
    </message>
    <message>
        <source>Memory size:</source>
        <translation>内存大小：</translation>
    </message>
    <message>
        <source>Half of available memory</source>
        <translation>可用内存的一半</translation>
    </message>
    <message>
        <source>All available memory</source>
        <translation>全部可用内存</translation>
    </message>
    <message>
        <source>Passes:</source>
        <translation>测试轮数：</translation>
    </message>
    <message>
        <source>Tests:</source>
        <translation>测试项目：</translation>
    </message>
    <message>
        <source>Start Test</source>
        <translation>开始测试</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>状态</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>就绪</translation>
    </message>
    <message>
        <source>Errors</source>
        <translation>错误记录</translation>
    </message>
    <message>
        <source>Address</source>
        <translation>地址</translation>
    </message>
    <message>
        <source>Expected</source>
        <translation>期望值</translation>
    </message>
    <message>
        <source>Actual</source>
        <translation>实际值</translation>
    </message>
    <message>
        <source>Test</source>
        <translation>测试</translation>
    </message>
    <message>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <source>Stopping...</source>
        <translation>正在停止…</translation>
    </message>
    <message>
        <source>Please select at least one test.</source>
        <translation>请至少选择一个测试项目。</translation>
    </message>
    <message>
        <source>Failed to allocate %1 of memory.</source>
        <translation>无法分配 %1 内存。</translation>
    </message>
    <message>
        <source>Testing %1 ...</source>
        <translation>正在测试 %1 …</translation>
    </message>
    <message>
        <source>Pass %1/%2 - %3</source>
        <translation>第 %1/%2 轮 - %3</translation>
    </message>
    <message>
        <source>Tested %1 / %2, %3 errors</source>
        <translation>已测试 %1 / %2，%3 个错误</translation>
    </message>
    <message>
        <source>PASS - %1 tested in %2 s, no errors found.</source>
        <translation>通过 - 测试 %1 用时 %2 秒，未发现错误。</translation>
    </message>
    <message>
        <source>FAIL - %1 errors detected.</source>
        <translation>失败 - 检测到 %1 个错误。</translation>
    </message>
</context>
</TS>
