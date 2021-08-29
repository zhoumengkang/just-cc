## 实现思路

域名由 `<host>:<port>`组成，上节我们已经获取到域名了，所以只需要用`:`切割处理即可
```c
char *parse_host(const char *domain)
{
	char *s = strchr(domain, ':');
	char *host = NULL;

	if (s != NULL) {
		host = strndup(domain, s - domain);
	} else {
		host = strndup(domain, strlen(domain));
	}

	printf("%s:%d host:\t%s\n", __FILE__, __LINE__, host);

	return host;
}

int parse_port(const char *host, const char *scheme)
{
	int port = 0;

	char *s = strchr(host, ':');

	if (s != NULL) {
		port = atoi(s + 1);	// 字符串转数字
	} else if (strcmp(scheme, "http") == 0) {
		port = 80;
	} else if (strcmp(scheme, "https") == 0) {
		port = 443;
	} else {
		exit(1);
	}

	printf("%s:%d port:\t%d\n", __FILE__, __LINE__, port);

	return port;
}
```
然后把`host`做一些字符串检测，去除掉非法域名
```c
// mengkang.net
// meng-kang.net
// meng-kang001.net

char tmp = 0; 

for (int n = 0; n < (int)strlen(host); n++) {
    tmp = host[n];
    if ((tmp >= 'a' && tmp <= 'z') ||
        (tmp >= '0' && tmp <= '9') || tmp == '.' || tmp == '-') {

    } else {
        printf("host 非法:%s\n", host);
        exit(1);
    }
}
```
#### atoi
```c
int atoi(const char *nptr);
```
把参数 `nptr` 所指向的字符串转换为一个整数（类型为 `int` 型）
#### strcmp
```c
int strcmp(const char *s1, const char *s2);
```
将字符串 `s1` 和 字符串`s1` 进行比较。返回值等于 0 表示相等

## 完整代码

https://gitee.com/zhoumengkang/just-cc/blob/master/code/1004