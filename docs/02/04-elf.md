上一节中，我们使用 gcc 编译器来编译源代码，最后生成的可执行文件，它的运行不再依赖 C 语言的环境。这和 Java 不一样的地方，Java 虽然也需要编译，但是最终生成的 Java 字节码，运行环境依赖 Java 的虚拟机环境来解释字节码。PHP 也类似，PHP 则是在运行时编译生成`opcode`，它的解释也依赖于 PHP 虚拟机。

gcc 生成的可执行文件在各种 UNIX 系统中都采用的 ELF 格式。ELF 原意为 EXtensible Link Format ，可扩展链接器格式，现在代表 Executable and Link Format ，可执行文件和链接格式。所以除了 gcc 生成的最终的可执行文件，还有 gcc 生成的目标文件， gcc 生成的共享库，也都是 ELF 格式。

可以使用`readelf`工具查看，这里只截取了 ELF 头信息

```bash
$ readelf -a a.out
ELF 头：
  Magic：  7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (可执行文件)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  入口点地址：              0x400440
  程序头起点：              64 (bytes into file)
  Start of section headers:          7288 (bytes into file)
  标志：             0x0
  本头的大小：       64 (字节)
  程序头大小：       56 (字节)
  Number of program headers:         9
  节头大小：         64 (字节)
  节头数量：         35
  字符串表索引节头： 32

...
```

ELF 文件的具体分析，我们后面再学习。