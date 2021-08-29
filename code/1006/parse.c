#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

char *parse_host(const char *domain)
{
	char *s = strchr(domain, ':');
	char *host = NULL;
	char tmp = 0;

	if (s != NULL) {
		host = strndup(domain, s - domain);
	} else {
		host = strndup(domain, strlen(domain));
	}

	for (int n = 0; n < (int)strlen(host); n++) {
		tmp = host[n];
		if ((tmp >= 'a' && tmp <= 'z') ||
		    (tmp >= '0' && tmp <= '9') || tmp == '.' || tmp == '-') {

		} else {
			printf("非法 host:%s\n", host);
			exit(1);
		}
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

char *parse_ip(const char *host_name)
{
    struct hostent *hostent = gethostbyname(host_name);
    char *ip = inet_ntoa(*(struct in_addr *)hostent->h_addr_list[0]);

    printf("%s:%d ip:\t%s\n", __FILE__, __LINE__, ip);

    return strndup(ip, strlen(ip));
}

char *parse_uri(const char *url, const char *scheme, const char *domain)
{
	url = url + strlen(scheme) + strlen("://") + strlen(domain);
	char *uri = strndup(url, strlen(url));

    printf("%s:%d uri:\t%s\n", __FILE__, __LINE__, uri);

	return uri;
}
