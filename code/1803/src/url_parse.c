#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "url_parse.h"
#include "log.h"

char *parse_scheme(const char *url)
{
	const char *p = strstr(url, "://");

	if (p == NULL) {
		error_log("url 非法\t%s\n", url);
		exit(1);
	}

	long len = p - url;

	char *scheme = strndup(url, len);

	debug_log("scheme:\t%s\n", scheme);

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

	debug_log("domain:\t%s\n", domain);

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
			error_log("非法 host:%s\n", host);
			exit(1);
		}
	}

	debug_log("host:\t%s\n", host);

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

	debug_log("port:\t%d\n", port);

	return port;
}

char *parse_ip(const char *host_name)
{
	struct hostent *hostent = gethostbyname(host_name);
	char *tmp = inet_ntoa(*(struct in_addr *)hostent->h_addr_list[0]);
	char *ip = strndup(tmp, strlen(tmp));;;

	debug_log("ip:\t%s\n", ip);

	return ip;
}

char *parse_uri(const char *url, const char *scheme, const char *domain)
{
	url = url + strlen(scheme) + strlen("://") + strlen(domain);
	char *uri = strndup(url, strlen(url));

	debug_log("uri:\t%s\n", uri);

	return uri;
}
