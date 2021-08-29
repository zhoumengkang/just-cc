承接上一节的问题，我们做如下修改，去掉`Hello world\n`后面的换行符
```c
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("Hello world");
	write(1, "mywget\n", 7);
	return 0;
}
```
```bash
$ ./stdout_demo
mywget
Hello world$
```
在终端作为标准输出的情况下，`Hello world`也在`mywget`之后了。这是什么缘故呢？

## C 标准库的 I/O 缓冲区

不知道大家是否很好的区分了缓冲和缓存，缓存是为了快速读取；缓冲是为了减少操作的频次。

比如人类的大肠可以对粪便的排泄做一些缓冲，堆积到一定量了再排便；而鸡是直肠动物，不能存储粪便，没有了缓冲区，导致鸡频繁的排便。

用户程序使用`C` 标准库的`I/O`函数来读取文件或设备，最终实现都依赖于系统调用将读写请求传递给内核，最终由内核驱动磁盘或设备完成 `I/O` 操作。为了系统更高效的运转，减少系统调用的次数是非常常见且重要的优化手段。

`C` 标准库为每个打开的文件分配一个 `I/O` 缓冲区以加速读写操作。用户调用读写函数大多数时候都在`I/O` 缓冲区中读写。当`I/O`缓冲区写满了，才会通过系统调用把`I/O`缓冲区中的数据传给内核。

C标准库的I/O缓冲区有三种类型：全缓冲、行缓冲和无缓冲。当用户程序调用库函数做写操作时，不同类型的缓冲区具有不同的特性。

## strace 验证行缓冲和全缓冲

**标准输出对应终端设备时是行缓冲**，所以当我们使用终端输出时，`printf("Hello world\n")`是执行完就立即刷新缓冲区；而不带`\n`的情况，要等到程序退出时才会刷新缓冲区，所以在最后输出。

```c
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("a\n");
	printf("b");
	printf("c");
	write(1, "d", 1);
	return 0;
}
```
编译运行
```bash
$ ./stdout_demo
a
dbc$
```
通过`strace`可以查看到系统调用的详细逻辑
```bash
$ strace ./stdout_demo
execve("./stdout_demo", ["./stdout_demo"], 0x7ffc13381ee0 /* 23 vars */) = 0
...
write(1, "a\n", 2a
)                      = 2
write(1, "d", 1d)                        = 1
write(1, "bc", 2bc)                       = 2
exit_group(0)                           = ?
+++ exited with 0 +++
```
`<stdio.h>`下的函数才设计有 `I/O` 缓冲区，而`write`函数依赖的头文件是`<unistd.h>`是 POSIX 操作系统 API。可以理解直接做系统调用。

因为是行缓冲，所以最后有3次`write`系统调用，且`bc`最后输出。

当**标准输出到文件时**，则是全缓冲，只有两次`write`系统调用了。
```bash
$ strace ./stdout_demo > a.txt
execve("./stdout_demo", ["./stdout_demo"], 0x7fff7aad2f90 /* 23 vars */) = 0
...
write(1, "d", 1)                        = 1
write(1, "a\nbc", 4)                    = 4
exit_group(0)                           = ?
+++ exited with 0 +++
$  cat a.txt
da
bc$
```
## fflush 刷新缓冲区
```c
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("Hello world\n");
	fflush(stdout);
	write(1, "mywget\n", 7);
	return 0;
}
```
```bash
$ ./stdout_demo > a.txt && cat a.txt
Hello world
mywget
```