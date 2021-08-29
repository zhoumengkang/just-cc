我们做业务，接触最多的可能就是：数值、数组、字符串、函数、逻辑处理。我们本章一起过一遍下整型和浮点型。

## 整型
整型包括 char、short int（或者简写为short）、int、long int（或者简写为long）、long long int（或者简写为long long）等几种。这些类型都可以加上signed或unsigned关键字表示有符号或无符号数。比较特殊的是`char` 字符类型。char型占一个字节的存储空间，一个字节通常是8个bit。如果这8个bit按无符号整数来解释，取值范围是`0~255`，如果按有符号整数来解释，取值范围是`-128~127`。 因为我们现在都是用 64 位操作系统，`long` 就可以取代 `long long` 了，占用内存和取值范围都一致。

在标准库头文件`<limits.h>`中定义了本地实现中每种类型的最大值和最小值。在线地址 https://zh.cppreference.com/w/c/types/limits 

```c
#include <stdio.h>

int main(void)
{
    char c1 = -127;
    unsigned char c2 = 255;

    short int s1 = -32768;
    short s2 = 32767;
    unsigned short s3 = 65535;

    int i1 = -2147483648;
    unsigned int i2 = 4294967295;

    long int l1 = -9223372036854775808;
    long l2 = +9223372036854775807;
    unsigned long l3 = 18446744073709551615;

    printf("sizeof(char)\t%lu\n",sizeof(c1));
    printf("sizeof(unsigned char)\t%lu\n",sizeof(c2));

    printf("sizeof(short int)\t%lu\n",sizeof(s1));
    printf("sizeof(short)\t%lu\n",sizeof(s2));
    printf("sizeof(unsigned short)\t%lu\n",sizeof(s3));

    printf("sizeof(int)\t%lu\n",sizeof(i1));
    printf("sizeof(unsigned int)\t%lu\n",sizeof(i2));

    printf("sizeof(long int)\t%lu\n",sizeof(l1));
    printf("sizeof(long)\t%lu\n",sizeof(l2));
    printf("sizeof(unsigned long)\t%lu\n",sizeof(l3));

    return 0;
}
```

## 浮点型
```c
#include <stdio.h>

int main()
{
    float num1 = 3.1415926535;
    double num2 = 3.1415926535;

    printf("num1 = %.10f, sizeof(float) = %lu\n", num1, sizeof(num1));
    printf("num2 = %.10f, sizeof(double) = %lu\n", num2, sizeof(num2));

    return 0;
}
```
编译运行
```bash
$ gcc float.c
$ ./a.out
num1 = 3.1415927410, sizeof(float) = 4
num2 = 3.1415926535, sizeof(double) = 8
```
`double` 精度要比 `float` 高

## 总结
数据类型 | 说明 | 字节
------- | ------- | -------  
char | 字符型 | 1
short|短整型|2
int | 整型| 4
float|单精度浮点型|4
double|双精度浮点型|8
long|长整型|8
