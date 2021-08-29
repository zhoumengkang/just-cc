#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>		/* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "log.h"
#include "url_parse.h"
#include "http_download.h"

char *http_create_request(http_url * url)
{
	const char *request_tpl =
	    "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n";

	int size =
	    strlen(request_tpl) + strlen(url->uri) + strlen(url->host) - 4 + 1;

	char *http_request = (char *)malloc(size);

	sprintf(http_request, request_tpl, url->uri, url->host);

	debug_log("http_request\t%s\n", http_request);

	return http_request;
}

int http_connect(http_url * url)
{
	// create
	int sockfd = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		exit(1);
	}
	// connect
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(url->port);

	if (inet_pton(AF_INET, url->ip, &servaddr.sin_addr) < 0) {
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		exit(1);
	}
	// write
	char *http_request = http_create_request(url);

	if (write(sockfd, http_request, strlen(http_request)) < 0) {
		exit(1);
	}

	free(http_request);

	return sockfd;
}

static char *realloc_response(char *response, int size)
{

	char *response_new = (char *)realloc(response, size);

	if (!response_new) {
		error_log("realloc error\n");
		exit(1);
	}

	if (response_new != response) {
		free(response);
		response = response_new;
	}

	return response;
}

int http_download(int sockfd, const char *filename)
{
	// read
	int buf_size = 128;
	int response_size = buf_size * 4;
	int len = 0;
	int header_length = 0;
	bool is_body = false;

	char response_buf[buf_size];
	memset(response_buf, 0, buf_size);

	char *response = (char *)malloc(response_size);
	memset(response, 0, response_size);

	int fd = open(filename, O_CREAT | O_WRONLY, 0777);

	while ((len = read(sockfd, response_buf, buf_size)) > 0) {

		if (is_body) {
			write(fd, response_buf, len);
			continue;
		}

		if (header_length + len >= response_size) {
			response_size *= 2;
			response = realloc_response(response, response_size);
		}

		response_buf[len] = '\0';
		strcat(response, response_buf);

		char *header_end = strstr(response, "\r\n\r\n");

		if (!header_end) {
			continue;
		}

		is_body = true;

		header_length = header_end - response;
		char *response_header = strndup(response, header_length);
		printf("%s\n", response_header);
		free(response_header);

		char *response_body = response + header_length + 4;

		write(fd, response_body, strlen(response_body));

		int response_buf_len = strlen(response_buf);

		if (response_buf_len < len) {
			char *padding = response_buf + response_buf_len;
			write(fd, padding, len - response_buf_len);
		}
	}

	free(response);

	close(sockfd);

	return 0;
}
