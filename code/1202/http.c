#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>		/* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include "log.h"
#include "parse.h"
#include "http.h"

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

int http_download(int sockfd)
{
	// read
	int buf_size = 128;
	int response_size = buf_size * 4;
	int len = 0;
	int header_length = 0;

	char response_buf[buf_size];
	memset(response_buf, 0, buf_size);

	char *response = (char *)malloc(response_size);
	memset(response, 0, response_size);

	while ((len = read(sockfd, response_buf, buf_size)) > 0) {

		if (header_length + len >= response_size) {
			response_size *= 2;
			char *response_new =
			    (char *)realloc(response, response_size);

			if (!response_new) {
				error_log("realloc error\n");
				exit(1);
			}

			if (response_new != response) {
				free(response);
				response = response_new;
			}
		}

		response_buf[len] = '\0';
		strcat(response, response_buf);

		char *header_end = strstr(response, "\r\n\r\n");

		if (header_end) {
			int offset = header_end - response;
			response[offset] = '\0';
			printf("%s\n", response);
			break;
		}

		header_length += len;
	}

	free(response);

	// close
	close(sockfd);

	return 0;
}
