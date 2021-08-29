承接上一节的函数定义与声明，说下函数指针。
为什么要说函数指针呢，因为C里面的函数调用方式`花样`很多。我们先学习下面的函数指针，然后学习回调函数，就知道它的用途了。

## 打印函数首地址
C语言中：函数名代表函数的首地址，这就是定义。类似的，数组名代表数组的首地址。指针也就是首地址。
```c
#include <stdio.h>

void func(int); // 形参名可忽略，上节说过

int main(int argc, const char *argv[])
{
    printf("fun addr is %p\n", func); // 函数名代表函数的首地址，这就是定义
    return 0;
}

void func(int i)
{
    printf("%d\n", i);
}
```  
运行结果
```js
fun addr is 0x400557
```  
## 函数指针的赋值

```c
#include <stdio.h>

void func(int);

int main(int argc, const char *argv[])
{
	printf("fun addr is %p\n", func);
	
	void (*fp)(int); // 声明一个函数指针
	fp = func; // 把func的首地址赋值给该指针
	printf("fp addr is %p\n", fp); // 所以fp的值和func一致
	fp(5); // 使用函数指针（本身也就是函数名）来调用函数
	
	return 0;
}

void func(int i)
{
	printf("%d\n", i);
}
```
是不是莫名其妙呢？为什么要弄这么绕，一顿操作，不和直接使用`func`一样么？不要急，后面一节，回调函数就明了了，先记住这么用。

## 函数指针的赋值规则

也就是把函数定义里面的形参删掉，留下参数类型，然后给函数名加上一个`*`指针的标识，然后加一个括号。
```c
void func(int i)
```
指针定义
```c
void (*fp)(int)
```

> 上例中，实际测试中发现使用`void (*fp)()`也能通过编译，运行结果也正常，可能是编译器的优化吧。

如果参数多一个呢？
```c
void func(int i, int j)
```
那么指针定义
```c
void (*fp)(int, int)
```