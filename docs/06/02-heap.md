以下演示依赖以下两个函数，使用时必须引入`<stdlib.h>`头文件

函数 | 作用
----- | -----
`void *malloc(int num)` | 在堆区分配一块指定大小的内存空间，用来存放数据。这块内存空间在函数执行完成后不会被初始化，它们的值是未知的
`void free(void *address)` | 释放 address 所指向的内存块,释放的是动态分配的内存空间


```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *a = (char *)malloc(4);
    char *b = (char *)malloc(4);
    
    printf("a 地址:\t%p\n",&a);
    printf("b 地址:\t%p\n",&b);
    printf("a 的值:\t%p\n",a);
    printf("b 的值:\t%p\n",b);

    free(a);
    free(b);

    return 0;
}
```
编译运行
```bash
a 地址:	0x7ffd5a268108
b 地址:	0x7ffd5a268100
a 的值:	0x2300010
b 的值:	0x2300030
```

> 配置 man 插件有助于我们编写代码时，快速定位头文件，和查看函数定义，具体可以看 附录/VIM/查看手册


- `char *a`，变量是`a`,`char *`只是告诉编译器，这个指针标记的数据是`char`类型。
- 既然是变量，这个变量就是在栈上申请内存，而且是自动申请，自动释放，所以我们打印`&a`的指针地址`0x7ffd5a268108`，就和上一节栈地址比较相似。
- 变量`a`里面存储的值则是`malloc`从堆上申请的内存的首地址`0x2300010`
- 通过比较可以发现堆的空间是向上生长的

