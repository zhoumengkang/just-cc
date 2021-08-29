从作用域上来说，C语言可以定义4种不同的变量：全局变量，静态全局变量，局部变量，静态局部变量。 

下面仅从函数作用域的角度分析一下不同的变量，假设所有变量声明不重名。 

- **全局变量**
    在函数外声明，例如，`int gVar;`。全局变量，所有函数共享，在任何地方出现这个变量名都是指这个变量。 除非在函数内部被重新声明定义。
- **静态全局变量**
    在函数外声明，例如：`static int sgVar;`。只在定义该变量的源文件内有效，在其它源文件中不能使用它。
- **局部变量**
    在函数中或者块内声明，例如函数/块内的`int var;`。不共享，函数的多次执行中涉及的这个变量都是相互独立的，他们只是重名的不同变量而已。 
- **局部静态变量**
    在函数中或者块内声明，例如函数中的`static int sVar;`。本函数内共享，函数的每一次执行中涉及的这个变量都是这个同一个变量。

## 全局变量
```c
#include <stdio.h>

int gVar = 1; // 全局变量

void gVarInr();

int main()
{
    printf("gVar is %d\n",gVar); // 1
    gVarInr();
    printf("gVar is %d\n",gVar); // 2
    return 0;
}

void gVarInr()
{
    gVar++;
    printf("gVar is %d\n",gVar); // 2
}
```
编译运行
```bash
gVar is 1
gVar is 2
gVar is 2
```
如果我们在`main`函数里初始化一个变量，变量名和全局变量名保持一致
```diff
#include <stdio.h>

int gVar = 1; // 全局变量

void gVarInr();

int main()
{
+   int gVar = 10;
    printf("gVar is %d\n",gVar); // 10
    gVarInr();
    printf("gVar is %d\n",gVar); // 10
    return 0;
}

void gVarInr()
{
    gVar++;
    printf("gVar is %d\n",gVar); // 2
}
```  
编译运行
```
gVar is 10
gVar is 2
gVar is 10
```  
所以全局变量所有函数共享，在任何地方出现这个变量名都是指这个变量。除非在函数内部被重新声明定义。

## 全局静态变量
全局变量之前再冠以 static 就构成了静态的全局变量。

全局变量本身就是**静态存储方式**， 静态全局变量当然也是静态存储方式。这两者在存储方式上并无不同。

这两者的区别在于**非静态**的全局变量的作用域是整个源程序，当一个源程序由多个源文件组成时，**非静态**的全局变量在各个源文件中都是有效的。而静态全局变量则限制了其作用域，即只在定义该变量的源文件内有效，在同一源程序的其它源文件中不能使用它。

`a.c`代码
```c
#include <stdio.h>

int a = 10;
```  
`b.c`代码
```c
#include <stdio.h>
extern int a;

int main(int argc, const char *argv[]) {
  printf("%d\n", a);
  return 0;
}
```

在`b.c`使用 `extern` 引入`a.c`里面定义的全局变量`a`。编译
```
$ gcc a.c b.c -o test
$ ./test
10
```  
修改`a.c`代码为全局静态变量
```diff
#include <stdio.h>

- int a = 10;
+ static int a = 10;
```
再编译则提示
```bash
$ gcc a.c b.c -o test
Undefined symbols for architecture x86_64:
  "_a", referenced from:
      _main in bbb-616389.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```  

扩展：函数和静态函数也与之类似，函数默认是全局的，加了`static`之后，其作用域就只是本文件了。

## 局部变量
```c
#include <stdio.h>

void varInr();

int main() {
    
    varInr();
    varInr();

    return 0;
}

void varInr()
{
    int var = 0;
    var++;
    printf("var is %d\n",var);
}
```  
编译运行
```bash
var is 1
var is 1
```  
这个大家都很熟悉了

## 局部静态变量
```diff
#include <stdio.h>

void varInr();
int main() {

    varInr();
    varInr();

    return 0;
}

void varInr()
{
-   int var = 0;
+   static int var = 0;
    var++;
    printf("var is %d\n",var);
}
```  
编译运行
```bash
var is 1
var is 2
```  

## 多线程
> 可以先跳过

上面几种作用域都是从函数的角度来定义作用域的，可以满足所有我们对单线程编程中变量的共享情况。
现在我们来分析一下多线程的情况。在多线程中，多个线程共享除函数调用栈之外的其他资源。 因此上面几种作用域从定义来看就变成了：
- 全局变量，所有函数共享，因此所有的线程共享，不同线程中出现的不同变量都是这同一个变量。
- 静态全局变量，所有函数共享，也是所有线程共享。 
- 局部变量，此函数的各次执行中涉及的这个变量没有联系，因此，也是各个线程间也是不共享的。 
- 静态局部变量，本函数间共享，函数的每次执行涉及的这个变量都是同一个变量，因此是各个线程共享的。