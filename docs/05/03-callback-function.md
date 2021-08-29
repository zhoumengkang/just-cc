上节课中，我们学会了函数指针的声明和函数指针调用函数的方法。
这节课，我们学习函数指针在回调函数中的妙用。
回调函数就是参数名中包含有函数指针的函数。比如
```c
void call(void (*fp)(int));
```  
接着上节课的说明
```c
#include <stdio.h>

void func(int);
void call(void (*fp)(int)); // 把 func 的函数指针作为 call 的参数

int main(int argc, const char *argv[])
{
	call(func); // 直接使用原始函数名，函数名就是函数的首地址
	
	return 0;
}

void func(int i)
{
	printf("%d\n", i);
}

void call(void (*fp)(int))
{
	fp(10);
}
```  
上面的函数调用的套路是不是熟悉呢，这就是C语言里面的回调函数的执行方式，必须传入函数指针。

上面的例子复杂化点
```c
#include <stdio.h>

int sum(int a, int b);

int call(int (*p)(int, int), int a, int b);


int main(int argc, const char *argv[]) {
    sum(1, 2);
    call(sum, 3, 4);
    return 0;
}

int sum(int a, int b) {
    printf("%d\n", a + b);
    return 0;
}

int call(int (*p)(int, int), int a, int b) {
    return p(a, b);
}
```  
