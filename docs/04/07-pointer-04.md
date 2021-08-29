很多时候，我们无法确定数据类型，不管是传入的参数还是返回值。

比如 java 里面 api 返回体中的数据，经常会用泛型，因为每个api返回的数据实体都不一样，无法确定。
C 语言里面也有类似的情况，所有会有泛型指针，来切换指针的类型。比如我们在堆内存上申请空间的时候，使用`malloc`
```c
void * malloc(size_t size);
```  
因为我们不知道这个内存申请完了以后用来存放什么，所以返回的是泛型指针。也就可以存放任何类型行的数据了。

```c
#include <stdio.h>

int main(int argc, const char *argv[]) {

    void *p;
    int a = 1;
    p = &a;
    printf("a: %d\n",*p);

    return 0;
}
```
```bash
$ gcc main.c
main.c: 在函数‘main’中:
main.c:8:23: 警告：提领‘void *’指针 [默认启用]
     printf("a: %d\n", *p);
                       ^
main.c:8:5: 错误：对 void 表达式的使用无效
     printf("a: %d\n", *p);
     ^
```
优化下
```c
#include <stdio.h>

int main(int argc, const char *argv[]) {

    void *p;
    
    int a = 1;
    
    p = &a; // 把变量a的地址存放到p中
    printf("a: %d\n",*(int *)p); // 如果要打印出a的值,那么就需要把p指针首选转换 int 类型的指针,然后通过 * 来解引,获取真实的值

    return 0;
}
```  
增加字符串
```c
#include <stdio.h>

int main(int argc, const char *argv[]) {

    void *p;

    int a = 1;

    p = &a; // 把变量a的地址存放到p中
    printf("a: %d\n",*(int *)p); // 如果要打印出a的值,那么就需要把p指针首选转换 int 类型的指针,然后通过 * 来解引,获取真实的值

    char *c = "1111";

    p = c;
    printf("c: %s\n", p); // 不会报错
    printf("c: %s\n", (char *)p); // 这样更加易读

    return 0;
}
```  
为什么不是
```c
printf("c: %s\n", *(char *)p);
```  
因为`printf`的`%s`参数就是`char *`指针。



