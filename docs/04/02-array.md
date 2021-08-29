## 什么是数组

数组就是存储在一片连续的内存空间的同一类型多个元素的有序列表。

以整型数组为例，我们新建一个`array.c`文件
```c
#include <stdio.h>

int main()
{
    int number[5] = {1, 2, 3};

    for (int i = 0; i < 5; ++i)
    {
        printf("number[%d] value: %d\t address: %p\n", i, number[i], &number[i]);
    }

    printf("sizeof(number) = %lu\n", sizeof(number));

    return 0;
}
```

编译运行
```bash
$ gcc -Wall -g -std=gnu99 array.c
$ ./a.out
number[0] value: 1	 address: 0x7ffebeba8b70
number[1] value: 2	 address: 0x7ffebeba8b74
number[2] value: 3	 address: 0x7ffebeba8b78
number[3] value: 0	 address: 0x7ffebeba8b7c
number[4] value: 0	 address: 0x7ffebeba8b80
sizeof(number) = 20
```

![int 数组 的内存分布](../img/c/array.jpg)

?> 一个地址所对应的内存单元只能存一个字节，所以 int 数据类型保存在内存中要占用连续的4个地址

## 数组初始化方式

初始化 | 内存占用 | 描述
------- | ------- | -------  
`int num1[3] = {1,2,3};` | 12 | 正常赋值
`int num2[3] = {1,2};` | 12 | 没有初始化的元素，填充为0。
`double num3[3] = {1,2};` | 24 | 不足的元素，填充为`0.0000...` , 数组类型声明，对其进行转换
`double num4[] = {1,2};` | 16 |  确定了元素个数，可以省略中括号里面的元素个数的描述
`int num5[5];` | 20 |  确定了元素个数，未初始化也申请了内存

## 编译运行小技巧

因为我们目前写的代码都是学习性质的，类似于单元测试类的，单文件的代码，编译比较简单，可以通过配置 vim 的方式来省去下面的操作。
```bash
$ gcc -Wall -g -std=gnu99 array.c
$ ./a.out
```
具体参考 附录/VIM/编译运行小技巧