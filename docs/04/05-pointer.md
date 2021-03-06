## 指针的定义

指针是一个变量，存在栈上。它的作用是用来存放另一个变量的地址。

每一个数据类型都会对应一个数据类型的指针，虽然都是地址，但是为了编译器方便，需要我们在使用时说明该指针的类型。当然有时候也可以使用泛指针，因为有时候无法确定指针的类型，有时候需要做指针类型的转换。

```c
type *name
```  
type | 指针的类型
------- | ------- 
name | 变量名

比如
```c
int *a;
char *b;
```  

!> 误区，不要认为`*name`是指针变量，其实`name`才是指针变量，`*`只是告诉编译器，后面的变量是指针类型。也可以加空格`int * a`，也可以是`int* a`。

## 指针的使用
```c
#include <stdio.h>

int main() {
    int var = 100;
    int *p;

    p = &var;  // & 取地址

    printf("var 的地址: %p\n", &var);

    printf("p 的值: %p\n", p);

    printf("*p: %d\n", *p); // * 加指针，表示取指针对应的地址的值

    printf("*(&var): %d\n", *(&var)); // 首选取var的地址，然后再通过*来取出该地址的值

    *p = 101; // *p 可以直接赋值

    printf("赋值后 *p: %d\n", *p);
    printf("赋值后 var: %d\n", var);

    return 0;
}
```  
编译运行
```bash
var 的地址: 0x7fff8abe0554
p 的值: 0x7fff8abe0554
*p: 100
*(&var): 100
赋值后 *p: 101
赋值后 var: 101
```  
小结
- `&` 是取地址符
- `*(&var)` 首选取var的地址，然后再通过*来取出该地址的值
- `*p` 可以直接赋值

## 指针赋值
上面的例子中，我们做了赋值操作`*p = 101`，下面我们再测试下赋值操作
```c
#include <stdio.h>

int main()
{
    int *p;
 
    *p = 101;
 
    printf("%d\n", *p);
 
    return 0;
}
```
> 这里使用我们之前设置的快捷编译没有输出结果，我们退出vim，执行下
```bash
$ gcc -Wall -g -std=gnu99 3.c
3.c: 在函数‘main’中:
3.c:7:8: 警告：此函数中的‘a’在使用前未初始化 [-Wuninitialized]
     *p = 101;
        ^
$ ./a.out
段错误
```
我们使用`malloc`分配内存，然后即可赋值。
```c
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *p;
    p = (int *)malloc(sizeof(int));
    *p = 101;
 
    printf("%d\n", *p);
    
    free(p);
 
    return 0;
}
```

二者做下比较`代码1`中`p = &var`是，先有了变量`var`，而整型是自动在栈上申请内存的，所以是现有内存之后，把该地址关联到了指针`p`上；但是`代码2`中，声明了指针变量`p`，但是没有分配内存空间，所以不能直接给他赋值（即 101 这个整型数字不知道放在内存的什么地方）。

> 这块有点超纲，不能理解可以先跳过

## 思考题

下面的代码能通过编译吗？
```c
double a = 10.1;
int *p = &a;
```  



