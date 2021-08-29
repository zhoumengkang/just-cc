## URL 的组成部分
```bash
<scheme>://<user>:<password>@<host>:<port>/<url-path>?<query>#<fragment>
```
详细定义查看 https://www.ietf.org/rfc/rfc1738.txt



```bash
typedef struct my_url {
	char *scheme;
	char *user;
	char *pass;
	char *host;
	unsigned short port;
	char *path;
	char *query;
	char *fragment;
} my_url;
```
> 直接复制 php 里`php_url`