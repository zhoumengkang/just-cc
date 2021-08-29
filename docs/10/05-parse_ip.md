#### gethostbyname
```c
struct hostent *gethostbyname(const char *name);
```
根据 `host` 反查 `ip` 这就需要依托DNS解析。查看手册，需要引入以下头文件
```c
#include <netdb.h>
```
```c
struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
}
```
可以看到`h_addr_list`是一个`char`类型的二级指针，因为不知道其数组长度，只知道其最少有一个元素，所以可以使用`hostent->h_addr_list[0]`也可以使用`*(hostent->h_addr_list)`

#### inet_ntoa
```c
char *inet_ntoa(struct in_addr in);
```
将网络地址转换成“.”点隔的字符串格式。查看手册，需要引入以下头文件
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
```

测试如下代码
```c
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

char *parse_ip(const char *host_name)
{
    struct hostent *hostent = gethostbyname(host_name);
    char *ip = inet_ntoa(*(struct in_addr *)hostent->h_addr_list[0]);

    printf("%s:%d ip:\t%s\n", __FILE__, __LINE__, ip);

    return ip;
}

int main()
{
    char *ip = parse_ip("mengkang.net");
    free(ip);
    return 0;
}
```
编译运行
```bash
free(): invalid pointer: 0x00007feb28d4f718 ***
```

!> `inet_ntoa`返回的字符串，不是堆上的字符串（所以对返回值进行`free`会提示错误），而是在一个静态分配的缓冲区中，也就是栈上的`static`数组，后续调用将覆盖前面的调用。
```c
int main()
{
    char *ip1 = parse_ip("mengkang.net");
    char *ip2 = parse_ip("baidu.com");

    printf("%s:%d ip1:\t%s\n", __FILE__, __LINE__, ip1);
    printf("%s:%d ip2:\t%s\n", __FILE__, __LINE__, ip2);
    return 0;
}
```
编译运行发现函数外值被覆盖了
```bash
1.c:13 ip:	203.195.188.207
1.c:13 ip:	220.181.38.148
1.c:23 ip1:	220.181.38.148
1.c:24 ip2:	220.181.38.148
```
为了安全起见，我们在使用其返回值时，还是要复制一份到堆上
```c
char *parse_ip(const char *host_name)
{
    struct hostent *hostent = gethostbyname(host_name);
    char *ip = inet_ntoa(*(struct in_addr *)hostent->h_addr_list[0]);

    printf("%s:%d ip:\t%s\n", __FILE__, __LINE__, ip);

    return strndup(ip, strlen(ip));
}
```