## 指针的偏移
在第二节数组案例代码的基础上做些修改
```diff
#include <stdio.h>

int main()
{
    int number[5] = {1, 2, 3};
+   int *ptr;

+   ptr = number;

    for (int i = 0; i < 5; ++i)
    {
-       printf("number[%d] value: %d\t address: %p\n", i, number[i], &number[i]);
+       printf("*ptr value: %d\t ptr: %p\n", *ptr, ptr);
+       ptr++;
    }

    printf("sizeof(number) = %lu\n", sizeof(number));

    return 0;
}
```
- 新增一个指针变量 `ptr`
- `ptr` 里面存的是`number`数组的首地址，数组变量名即代表该数组的首地址
- 把循环打印的内容从数组下标改为了指针方式
- `ptr++` 移动数组指针到下一个成员的位置，如下图所示

![int 数组 的内存分布](../img/c/array.jpg)

编译运行
```bash
*ptr value: 1	 ptr: 0x7ffee8e49030
*ptr value: 2	 ptr: 0x7ffee8e49034
*ptr value: 3	 ptr: 0x7ffee8e49038
*ptr value: 0	 ptr: 0x7ffee8e4903c
*ptr value: 0	 ptr: 0x7ffee8e49040
```
截图里面的地址只是一个示例，每次运行结果不一。

通过上面的实例，我们知道

- 数组变量名即代表该数组的首地址
- 指针不管是`++`，还是`--`偏移的量都是该指针数据类型的单元大小


# 指针数组
根据[数组的定义](https://mengkang.net/1008.html)：同一类型多个元素的集合，所以数组也能存放指针集合。
```c
#include <stdio.h>

int main(int argc, char const *argv[]) {
    int a = 1;
    int b = 2;

    int *p[2] = {&a, &b};

    printf("a : %p,%d\n", &a, *(&a));
    printf("*(p + 0) : %p,%d\n", *(p + 0), *(*(p + 0)));

    printf("b : %p,%d\n", &b, *(&b));
    printf("*(p + 1) : %p,%d\n", *(p + 1), *(*(p + 1)));

    return 0;
}
```  
输出结果
```
a : 0x7fff55e0ca7c,1
*(p + 0) : 0x7fff55e0ca7c,1
b : 0x7fff55e0ca78,2
*(p + 1) : 0x7fff55e0ca78,2
```  
上例子，`p`表示数组，`int *`表示该数组存放的`int`类型的指针。依然数组指针偏移1位，是下一个成员的值。

# 数组指针

数组指针，指的是数组名的指针，即数组首元素地址的指针。（因为数组名就代表数组的首地址）

上面的例子关于`int *p[2]`的定义也可以写成`int* p[2]`，或者`int * p[2]`，但是不能写成`int (*p)[2]`。
因为在语法分析的时候`*`符号是和左边的数据类型结合的，不管中间有多少空格。而使用`()`之后，则使得`p`变成一个数组指针。

```c
#include <stdio.h>

int main(int argc, char const *argv[]) {

    int arr[2][2] = {{1,2},{3,4}};
    int (*p)[2] = arr; // p是一个指针变量，它指向一个数组,而这个数组的每个成员又是一个含有2个int的数组

    printf("%d\n", *(*(p+0)+0));//1
    printf("%d\n", **p);//1
    printf("%d\n", *(*(p+1)+1));//4

    return 0;
}
```  
这就是
```c
int *p[2]
int (*p)[2]
```  
的区别