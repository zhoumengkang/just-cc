```c
#include <stdio.h>

int main()
{
    int a = 1;
    int b = 1;
    int c = 1;

    printf("a:%p\n",&a);
    printf("b:%p\n",&b);
    printf("c:%p\n",&c);

    return 0;
}
```
编译运行
```bash
a:0x7ffdde0c620c
b:0x7ffdde0c6208
c:0x7ffdde0c6204
```
- 函数内的变量存放在栈上，运行时分配的空间，可以运行多次，地址都不一样。
- 栈上的内存是自动申请、自动回收，这点区别于堆上的内存使用
- 先声明的变量在地址值要大，也就是说栈空间的是向下生长的

```bash
$ cat /proc/cpuinfo |grep "address sizes"
address sizes	: 39 bits physical, 48 bits virtual
```

32 位操作系统的虚拟内存是4G，64位系统虚拟内存不是`2^64`的地址空间，而是`2^48`，所以类似于`0x7ffdde0c620c` 12位，而不是16位（16进制）
