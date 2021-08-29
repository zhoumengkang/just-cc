## 添加 parse_uri 函数
`domain`后面的就是`uri`，比较简单

```c
char *parse_uri(const char *url, const char *scheme, const char *domain)
{
	url = url + strlen(scheme) + strlen("://") + strlen(domain);
	char *uri = strndup(url, strlen(url));

	printf("%s:%d uri:\t%s\n", __FILE__, __LINE__, uri);

	return uri;
}
```

## 增加 http_url 结构体

这里偷懒，没有解析`path`、`query`、`fragment`，我们将这三者简写为`uri`（不适用于其他应用）。

在`parse.h`里添加`url`结构体，其他
```c
typedef struct {
	char *scheme;
	char *user;
	char *pass;
	char *domain;
	char *host;
	char *ip;
	char *uri;
	unsigned short port;
} http_url;
```
在`main.c`里添加结构体的初始化
```c
http_url *wget_url = (http_url *) malloc(sizeof(http_url));

wget_url->scheme = scheme;
wget_url->domain = domain;
wget_url->host = host;
wget_url->port = port;
wget_url->ip = ip;
wget_url->uri = uri;
```

到此域名的解析就基本完成了，完整版地址 

https://gitee.com/zhoumengkang/just-cc/blob/master/code/1006