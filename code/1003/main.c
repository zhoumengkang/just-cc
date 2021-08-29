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
