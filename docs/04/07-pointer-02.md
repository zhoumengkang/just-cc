## 二级指针的定义
也就是指向指针的指针，再通俗点就是，`a`是指针变量，`a`里面存了`b`的地址，而`b`也是一个指针。

```c
#include <stdio.h>

int main(int argc, const char *argv[])
{
	int i = 10;
	int *p = &i;		// p 里面存了i的地址
	int **p2 = &p;		// p2 里面又存了p的地址

	printf("%d\n", **p2);	    // 根据前面的规则 * 就是取该地址的值，这里最后输出10
	return 0;
}
``` 

## char *argv[] 与 char **argv

下面两种写法是等价的
```c
#include <stdio.h>

int main(int argc, const char *argv[])
{
	printf("%s", argv[0]);
	return 0;
}
```

```c
#include <stdio.h>

int main(int argc, const char **argv)
{
	printf("%s", argv[0]);
	return 0;
}
```
>《C 专家编程》P209 
在作为函数的参数时，数组的声明可以看作是一个指针，作为函数参数的数组，始终会被编译器修改为指向数组第一个元素的指针

所以`char *argv[]`会被编译成`char **argv`，`argc`参数很重要，记录着这个`argv`数组的长度

返回上例，我们增加一行
```diff
#include <stdio.h>

int main(int argc, const char *argv[])
{
	int i = 10;
	int *p = &i;		// p 里面存了i的地址
	int **p2 = &p;		// p2 里面又存了p的地址

	printf("%d\n", **p2);	    // 根据前面的规则 * 就是取该地址的值，这里最后输出 10
+   printf("%d\n", p2[0][0]);   // 同样也输出了 10
	return 0;
}
```
用`a[i]`这样的形式对数组进行访问总是被编译器"改写"为`*(a+i)`这样指针的访问，所以`p2[0][0]`等价于`*(*(p+0)+0)`

再绕下，我们在堆上为`int **p`申请空间
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int **p = (int **)malloc(sizeof(int *) * 2);

	int a[3] = { 1, 2, 3 };
	int b[3] = { 4, 5, 6 };

	p[0] = a;		// 数组下标的本质是指针根据指针类型步长的偏移，p 的首地址存放数组 a 的地址
	p[1] = b;		// p 的首地址向后偏移一位，存放 b 的地址

	printf("%d", **p);          // 1
	printf("%d", p[1][1]);      // 5
	printf("%d", *(*(p+1)+1));  // 5
	return 0;
}
```
`int **p`和上节`int (*p)[2]`对比，可以存储的内容都可以为是二位数组，但是后者知道数组的长度，`int **p`不知道，所以 `main`里第一个参数是参数个数 