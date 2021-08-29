在C语言中使用一个函数，必须声明它的返回值类型。由于C语言是强类型，所以函数的参数必须带有类型标识的。
下面说下C语言里面定义、声明函数的几种方式。

定义的方式 | 说明
------- | -------
函数定义声明 | 简单粗暴，简单测试使用
函数原型声明 | 适合单一文件，非常小的项目适用
在头文件里函数原型声明 | 如何你学习比较的开源项目，这种是最常见的，方便项目管理

下面都是定义了一个`test.c`文件

## 函数定义声明
```c
#include <stdio.h>

void func(int i)
{
    printf("%d\n", i);
}

int main(int argc, const char *argv[])
{
    func(1);
    return 0;
}
```  

## 函数原型声明
```c
#include <stdio.h>

void func(int i);// 声明

int main(int argc, const char *argv[])
{
	func(1);
	return 0;
}

void func(int i) // 定义
{
	printf("%d\n", i);
}
```  
变型
```diff
#include <stdio.h>

- void func(int i); // 声明
+ void func(int); // 没有行参名，只有参数类型也OK

int main(int argc, const char *argv[])
{
	func(1);
	return 0;
}

void func(int i) // 定义
{
	printf("%d\n", i);
}
```

## 在头文件里函数原型声明

```c
#include <stdio.h>
#include "test.h"

int main(int argc, const char *argv[])
{
	func(1);
	return 0;
}

void func(int i)
{
	printf("%d\n", i);
}
```  
只是不过把上面的函数原型声明，统一放置到了在同级目录下的 `test.h` 里面。
```c
void func(int i);
```  
当我们在做项目开发的时候，多使用最后一种方式，解释太多无意，抽象定义，方便管理。
