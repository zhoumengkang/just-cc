#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "url_parse.h"
#include "log.h"
#include "http_download.h"
#include "wget_opt.h"

int main(int argc, char *const argv[])
{
	int optc;
	const char *filename = NULL;

	while ((optc = getopt_long(argc, argv, "O:dVh", long_opts, NULL)) != -1) {
		//    printf("optc:%c\toptarg:%s\toptind:%d\n", optc, optarg, optind);

		switch (optc) {
		case 'O':
			filename = optarg;
			break;
		case 'd':
			set_debug(true);
			break;
		case 'V':
			printf("%s 0.0.1\n", PROGRAM_NAME);
			exit(0);
		case 'h':
			usage(0);
			break;
		case '?':
			usage(1);
			break;
		}
	}

	if (optind + 1 != argc) {
		usage(1);
	}

	const char *url = argv[optind];

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

	int fd = http_connect(wget_url);
	http_download(fd, filename);

	free(scheme);
	free(domain);
	free(host);
	free(ip);
	free(uri);

	free(wget_url);

	return 0;
}
