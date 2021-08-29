这里原封不动地搬运《The C Programming Language》里经典案例，用 C 语言打印出`hello, world`，我们首先必须编写程序文本，新建文件`hello.c`，代码如下
```c
#include <stdio.h>

void main()
{
	printf("hello, world\n");
}
```  

从上往下看，有三个知识点需要学习。

## 头文件

我们发现我们在代码文件最顶部使用了`#include <stdio.h>`，这是因为我们在使用的`printf`函数是在标准库`stdio.h`里面定义的。通过`include`来引入该头文件，我们可以通过 man 手册查看相关函数依赖的头文件
```bash
man 3 printf
```
来查阅
```bash
PRINTF(3)                  Linux Programmer's Manual                 PRINTF(3)

NAME
       printf,   fprintf,  sprintf,  snprintf,  vprintf,  vfprintf,  vsprintf,
       vsnprintf - formatted output conversion

SYNOPSIS
       #include <stdio.h>

       int printf(const char *format, ...);
       int fprintf(FILE *stream, const char *format, ...);
       int sprintf(char *str, const char *format, ...);
       int snprintf(char *str, size_t size, const char *format, ...);
```  
如上所示，也就是说在使用这四个函数的时候都需要包含`<stdio.h>`。

1. `#include <xxx.h>` 系统头文件
2. `#include "xxx.h"` 自定义头文件

## man 命令
顺便说下为什么是`man 3`，因为`man`查看手册的时候是分章节的，章节列表


章节编号|章节名称|章节主要内容
-----|----|----
1|General Commands|用户在shell中可以操作的指令或者可执行文档
2|System Calls|系统调用的函数与工具等
3|Sunroutines|C语言库函数
4|Special Files|设备或者特殊文件
5|File Formats|文件格式与规则
6|Games|游戏及其他
7|Macros and Conventions|表示宏、包及其他杂项
8|Maintenence Commands|表示系统管理员相关的命令

如果我输入的是
```sh
man printf
```  
出来的结果和`man 1 printf`一样，查的是 shell 命令里面的`printf`
```
PRINTF(1)                        User Commands                       PRINTF(1)

NAME
       printf - format and print data

SYNOPSIS
       printf FORMAT [ARGUMENT]...
       printf OPTION
... 
```  
man 是按照手册的章节号的顺序进行搜索的，所以我们在查C语言库函数的时候，记得使用`man 3 xxx`。

## main 函数

main 函数是整个程序的入口，所以编译一个项目，main 函数有且仅有有一个。这点其实 Java 等语言也都类似。
