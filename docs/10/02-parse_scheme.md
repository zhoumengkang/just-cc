## 实现思路
以处理 `http://mengkang.net/abc.jpg` 为例，要获取到`url`的协议，也就是`http`。
我们需要找到 `://` 第一次出现的位置，然后把这个位置之前的字符串截取出来或者复制出来。

字符串是我们做项目开发用到的最多的数据结构，结合实际的需求，我们就边学边用，记忆更加深刻。

!> 这一节极为重要，务必完全理解

## 使用 strtok
```c
char *strtok(char *str, const char *delim)
```
分解字符串 `str` 为一组字符串，`delim` 为分隔符。返回被分解的第一个子字符串。

新建文件`mywget.c`
```c
#include <stdio.h>
#include <string.h>

int main()
{
    char url[29] = "http://mengkang.net/abc.jpg";
    char *scheme = strtok(url, "://");
    printf("scheme:%s\n", scheme);
    return 0;
}
```
> 使用字符串函数时，我们必须先引入`<string.h>`头文件。
也可以用我前面讲到的 `map.vim`插件找到，`vimplus` 也会自动报错提醒我们

编译运行符合预期，不过这只是 demo，和实际使用场景不一样。实际我们是通过命令行传入的 url
```c
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const char *url = argv[1];
    printf("%s\n", url);

    char *scheme = strtok(url, "://");
    printf("scheme:%s\n", scheme);
    return 0;
}
```
保存完，发现系统提示`no matching function for call to 'strtok'`，但是我们明明已经使用过这个函数的呀，怎么回事呢，直接编译试试看
```bash
$ gcc -Wall -g -std=gnu99 mywget.c -O mywget
mywget.c: 在函数‘main’中:
mywget.c:12:5: 警告：passing argument 1 of ‘strtok’ discards ‘const’ qualifier from pointer target type [默认启用]
     char *scheme = strtok(url, "://");
     ^
In file included from mywget.c:2:0:
/usr/include/string.h:344:14: 附注：需要类型‘char * restrict’，但实参的类型为‘const char *’
 extern char *strtok (char *__restrict __s, const char *__restrict __delim)
```
哦，原来`strtok`第一个参数限制为`char *`，而我们传入的是`const char *`，所以编译都无法通过

!> **细节** `const char *` 与 `char *` 的区别

参数`const char *delim`表示入参`delim`是常量字符串，表示函数内部也不会对其进行修改，可以传入非常量字符串；而`strtok`函数中的第一个参数`char *str`前面没有带`const`说明在函数内部会对其内存做写操作，不能传入常量字符串

## 优化版本1 - 只读复制

知道了这个原因，也就知道如何优化了，思路只能是先得到常量字符串的长度，然后在栈上申请对应的内存长度的`char`数组，然后将常量字符串复制到该数组上。然后再切割就没问题了。

我们先补充学习几个函数

#### strlen
```c
size_t strlen(const char *str)
```
计算字符串 `str` 的长度，直到空结束字符，**但不包括空结束字符**。返回值是字符串长度

#### strncpy
```c
char *strncpy(char *dest, const char *src, size_t n)
```
把 `src` 所指向的字符串复制到 `dest`，最多复制 `n` 个字符。

呼应前面描述的`char *`与`const char *`的对比，大家可以体会下

#### memset
```c
void *memset(void *str, int c, size_t n)
```
复制字符 `c`（一个无符号字符）到参数 `str` 所指向的字符串的前 `n` 个字符。新申请的内存，不一定是干净的，一定要先清理。

#### 新版代码
有了上面的基础之后，我们编写如下代码

```c
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    // 这里我们简化处理，这传入两个参数 类似于 ./mywget http://mengkang.net/abc.jpg
    if (argc != 2) {
        return 1;
    }

    const char *url = argv[1];
    printf("%s\n", url);

    // 获取字符串长度，strlen 不含末尾的结束符，所以需要多申请一个字节的长度
    int len = strlen(url) + 1;

    char url_copy[len];

    memset(url_copy, 0, len);

    strncpy(url_copy, url, len);

    char *scheme = strtok(url_copy, "://");
    printf("scheme:%s\n", scheme);
    return 0;
}
```
编译运行
```bash
$ gcc -Wall -g -std=gnu99 mywget.c -o mywget
$ ./mywget http://mengkang.net/abc.jpg
http://mengkang.net/abc.jpg
scheme:http
```

#### 增加测试脚本
新建文件`test`，写入如下内容，并且`chmod +x test`
```bash
#!/bin/bash

set -x

ls *c *.h|xargs indent -linux

ls *.c|xargs gcc -Wall -g -std=gnu99 -o mywget

./mywget http://mengkang.net/abc.jpg
```
我们写完代码之后，即可在不退出`vim`状态下，直接执行`:!./test`，就可以编译并按照预定义参数执行程序了

## 优化版本2 - 封装函数
一个有品味有工匠精神的码农，肯定是不愿意写意大利面条。后面还有很多解析，我们得将其拆分成函数。
```c
#include <stdio.h>
#include <string.h>

char * parse_scheme(const char *url)
{
	int len = strlen(url) + 1;

	char url_copy[len];

	memset(url_copy, 0, len);

	strncpy(url_copy, url, len);

	char *scheme = strtok(url_copy, "://");

	printf("scheme:%s line:%d\n", scheme, __LINE__);

	return scheme;
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		return 1;
	}

	const char *url = argv[1];
	printf("%s\n", url);
	
	char *scheme = parse_scheme(url);

	printf("scheme:%s line:%d\n", scheme, __LINE__);
	return 0;
}
```
执行测试代码，输出也没问题，但是就真没问题吗？
```bash
gcc -Wall -g -std=gnu99 mywget.c -o mywget
./mywget http://mengkang.net/abc.jpg
http://mengkang.net/abc.jpg
scheme:http line:16
scheme:http line:32
```
我们增加一行
```diff
#include <stdio.h>
#include <string.h>

char * parse_scheme(const char *url)
{
	int len = strlen(url) + 1;

	char url_copy[len];

	memset(url_copy, 0, len);

	strncpy(url_copy, url, len);

	char *scheme = strtok(url_copy, "://");

	printf("scheme:%s line:%d\n", scheme, __LINE__);

	return scheme;
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		return 1;
	}

	const char *url = argv[1];
	printf("%s\n", url);
	
	char *scheme = parse_scheme(url);
+   parse_scheme("https://abc.com");

	printf("scheme:%s line:%d\n", scheme, __LINE__);
	return 0;
}
```
按照我们的预期，33行应该仍然是`http`，当我们再运行发现第33行输出的结果不一定是什么了
```bash
gcc -Wall -g -std=gnu99 mywget.c -o mywget
./mywget http://mengkang.net/abc.jpg
http://mengkang.net/abc.jpg
scheme:http line:16
scheme:https line:16
scheme: line:33
```
为什么呢？函数的运行时需要压栈和出栈，出栈的时候，函数内部申请的栈上的内存也都归还，不可以作为返回值。
具体的原因，可以参考《C专家编程》- 第6章 6.5 当函数被调用时发生了什么：过程活动记录；也可以参考 https://mengkang.net/1407.html

所以如果我们要把字符串作为返回值，只能使用堆上的内存了。而`strtok`本质上是把切割字符串替换了`\0`，我们在释放内存的时候，需要函数返回之前先释放后半部分的内存，`main`函数里释放前半部分内存。作为洁癖，有点浪费拦截，顺便我们新学习一个`strstr`函数来替换`strtok`。

```c
char *strstr(const char *haystack, const char *needle)
```
在字符串 `haystack` 中查找第一次出现字符串 `needle` 的位置，作为返回值。改写之后
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse_scheme(const char *url)
{
	const char *p = strstr(url, "://");

	if (p == NULL) {
		printf("url 非法\t%s\n", url);
		exit(1);
	}

	long len = p - url + 1;

	char *scheme = (char *)malloc(len);
	memset(scheme, 0, len);

	strncpy(scheme, url, len - 1);

	printf("scheme:%s line:%d\n", scheme, __LINE__);

	return scheme;
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		return 1;
	}

	const char *url = argv[1];
	printf("%s\n", url);

	char *scheme = parse_scheme(url);

	printf("scheme:%s line:%d\n", scheme, __LINE__);
	
	free(scheme);
	
	return 0;
}
```
先计算偏移量，按需拷贝，操作复杂。有没有更简单的方案呢，可以使用`strndup`，但是`strndup`**不是标准的C函数**，如果考虑跨平台的移植性，就不用再做下面的优化
```c
char *strndup(const char *s, size_t n);
```
复制`s`前`n`字符，且在新字符串末尾添加`\0`，修改之后如下
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse_scheme(const char *url)
{
	const char *p = strstr(url, "://");

	if (p == NULL) {
		printf("url 非法\t%s\n", url);
		exit(1);
	}

	long len = p - url;

	char *scheme = strndup(url, len);

	printf("scheme:%s line:%d\n", scheme, __LINE__);

	return scheme;
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		return 1;
	}

	const char *url = argv[1];
	printf("%s\n", url);

	char *scheme = parse_scheme(url);

	printf("scheme:%s line:%d\n", scheme, __LINE__);

	free(scheme);

	return 0;
}
```

## 总结
解析`scheme`分三步
1. 找到 `://` 第一次出现的位置, 计算与字符串首地址的偏移量
2. 根据偏移量在堆上申请内存,并初始化
3. 把只读区对应上的数据拷贝到新申请的内存上