## 实现思路
上节中我们处理 URL Scheme 的解析，这节开始处理 URL Doamin。

域名由 `<host>:<port>`组成，当我们做 Web 开发时，如果在`http://megkang.net`调用`http://megkang.net:8080`接口时，就会提示跨域，因为这是两个域名。

假如本地测试开发想让`megkang.net`访问本地`127.0.0.1`，我们需要绑定`host`，绑定`host`时是没有带上端口号的。

首先列举几种网址情况（下面的例子中先没有夹带端口，下面的`a.com`可以视为`a.com:443`）

情况 | 案例 | 说明
---- | ---- | ---- 
没有URL | `https://a.com` | `host`后面为空
后面有 URI | `https://a.com/bbbb` | `host`后面会有`/`
有 Query 参数 | `https://a.com?b=/c` | `host`后面为`?`，且参数里面不排除有`/`的可能性
有锚点 | `https://a.com#dddd` | `host`后面可能直接跟锚点标识`#`

所以，排除掉完整的 `URL` 前面已经知道的`Scheme`，获取`Doamin`的算法规则如下：

对`URL`后半部分按单字节循环，循环中匹配`{ '/', '?', '#' }`，以匹配到的最短的字符串为准，比如我们用`https://a.com?b=/c`这个 URL 去匹配，先匹配到`/`，然后才匹配到`?`，显然 `Doamin` 应该是`?`前面的字符串。如果全部检测完都没匹配上，则为第一种情况`https://a.com`，得到`Doamin`字符串之后，同理别忘了拷贝复制。

具体实现如下：
```c
/**
 * https://a.com/bbbb
 * https://a.com?b=/c
 * https://a.com#dddd
 * https://a.com
 */
char *parse_domain(const char *url, const char *scheme)
{
	url = url + strlen(scheme) + strlen("://");

	char a[3] = { '/', '?', '#' };
	int p = 0;
	int min = 0;
	char *s = NULL;

	for (int i = 0; i < 3; i++) {
		s = strchr(url, a[i]);
		if (s != NULL) {
			p = s - url;
			if (i == 0 || min > p) {
				min = p;
			}
		}
	}

	if (min == 0) {
		min = strlen(url);
	}

	char *domain = strndup(url, min);

	printf("%s:%d domain:\t%s\n", __FILE__, __LINE__, domain);

	return domain;
}
```

> 算法规则需要大家自己动手用自己熟悉的语言先实现一遍，可能就更好理解


## 模块化拆分
对上节的代码做一点简单的拆分优化，将 URL 的处理单独提出来，目录结构如下
```bash
.
├── main.c  // 主入口
├── parse.c // url 解析器具体实现
├── parse.h // url 头文件
└── test    // 测试脚本
``` 
文件 `parse.h`
```c
#ifndef MYWGT_PARSE_H
#define MYWGT_PARSE_H

char *parse_scheme(const char *url);
char *parse_domain(const char *url, const char *scheme);

#endif
```
> 加一个宏定义判断，这个在 C 语言中非常常见，兼容循环引入，重复引入头文件的情况

文件 `parse.c` 
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"

char *parse_scheme(const char *url)
{
	const char *p = strstr(url, "://");

	if (p == NULL) {
		printf("url 非法\t%s\n", url);
		exit(1);
	}

	long len = p - url;

	char *scheme = strndup(url, len);

	printf("%s:%d scheme:\t%s\n", __FILE__, __LINE__, scheme);

	return scheme;
}

/**
 * https://a.com/bbbb
 * https://a.com?b=/c
 * https://a.com#dddd
 * https://a.com
 */
char *parse_domain(const char *url, const char *scheme)
{
	url = url + strlen(scheme) + strlen("://");

	char a[3] = { '/', '?', '#' };
	int p = 0;
	int min = 0;
	char *s = NULL;

	for (int i = 0; i < 3; i++) {
		s = strchr(url, a[i]);
		if (s != NULL) {
			p = s - url;
			if (i == 0 || min > p) {
				min = p;
			}
		}
	}

	if (min == 0) {
		min = strlen(url);
	}

	char *domain = strndup(url, min);

	printf("%s:%d domain:\t%s\n", __FILE__, __LINE__, domain);

	return domain;
}
```
文件`main.c`
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		printf("%s\n", "请输入url");
		exit(1);
	}

	const char *url = argv[1];

	char *scheme = parse_scheme(url);
	char *domain = parse_domain(url, scheme);

	free(scheme);
	free(domain);
	return 0;
}
```
## 增加测试用例

文件`test`，编译命令做简单修改，修改了编译命令，并且增加几个测试用例
```bash
#!/bin/bash

set -x

ls *c *.h|xargs indent -linux

ls *.c|xargs gcc -Wall -g -std=gnu99 -o mywget


url_list=(
    "http://static.mengkang.net/?a=upload/image/2019/0907/1567834464450406.png"
    "https://static.mengkang.net?a=upload/image/2019/0907/1567834464450406.png"
    "https:/static.mengkang.net?a=upload/image/2019/0907/1567834464450406.png"
    "http://static.mengkang.net:8080/upload/image/2019/0907/1567834464450406.png?a=b#test"
    "static.mengkang.net"
)


for url in ${url_list[@]};do
    ./mywget ${url}
done
```
## 完整代码

https://gitee.com/zhoumengkang/just-cc/blob/master/code/1003