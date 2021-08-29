#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"
#include "log.h"

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		printf("%s\n", "请输入url");
		exit(1);
	}

	set_debug(true);

	const char *url = argv[1];

	char *scheme = parse_scheme(url);
	char *domain = parse_domain(url, scheme);
	char *host = parse_host(domain);
	int port = parse_port(domain, scheme);
	char *ip = parse_ip(host);
	char *uri = parse_uri(url, scheme, domain);

	http_url *wget_url = (http_url *) malloc(sizeof(http_url));

	wget_url->scheme = scheme;
	wget_url->domain = domain;
	wget_url->host = host;
	wget_url->port = port;
	wget_url->ip = ip;
	wget_url->uri = uri;

	free(scheme);
	free(domain);
	free(host);
	free(ip);
	free(uri);

	free(wget_url);

	return 0;
}
