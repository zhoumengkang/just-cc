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
