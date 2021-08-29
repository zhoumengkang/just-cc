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
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "log.h"
#include "url_parse.h"
#include "http_download.h"
#include "progress.h"
#include "util.h"

static long download_size = 0;
static long download_start_ms = 0;
static const char *download_filename;

static SSL_CTX *ctx;
static SSL *ssl;

char *http_create_request(http_url * url)
{
	const char *request_tpl =
	    "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n";

	int size =
	    strlen(request_tpl) + strlen(url->uri) + strlen(url->host) - 4 + 1;

	char *http_request = (char *)malloc(size);

	sprintf(http_request, request_tpl, url->uri, url->host);

	debug_log("%s\n", http_request);

	return http_request;
}

http_response_header *http_response_header_prase(const char *str, int len)
{
	char *header_str = strndup(str, len);

	http_response_header *http_res =
	    (http_response_header *) malloc(sizeof(http_response_header));

	const char *len_end = "\r\n";
	int line = 0;
	char *tmp = header_str;
	char *current_line;

	while ((current_line = strtok(tmp, len_end)) != NULL) {
		info_log("%s\n", current_line);
		tmp = NULL;
		line++;

		if (line == 1) {
			if (strcmp(current_line, "HTTP/1.1 200 OK") != 0) {
				error_log("http 状态码异常\n");
				exit(1);
			}

			http_res->http_code = 200;
			continue;
		}

		char *p = strchr(current_line, ':');

		if (!p)
			continue;

		if (strncmp
		    (current_line, "Content-Length", p - current_line) == 0) {
			http_res->content_length = atoi(p + 2);
		} else
		    if (strncmp
			(current_line, "Content-Type", p - current_line) == 0) {
			http_res->content_type = strdup(p + 2);
		}

	}

	free(header_str);
	printf("\n长度:%lu(%0.1fM)[%s]\n\n", http_res->content_length,
	       (float)http_res->content_length / (1024 * 1024),
	       http_res->content_type);

	return http_res;
}

void https_content_init(int fd, char *http_request)
{

	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);

	if (SSL_connect(ssl) == -1) {
		ERR_print_errors_fp(stderr);
	}

	int len = SSL_write(ssl, http_request, strlen(http_request));

	if (len < 0) {
		error_log("SSL Send failure! errno = %d, err_msg = %s\n", errno,
			  strerror(errno));
		exit(1);
	}
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

	char *http_request = http_create_request(url);

	if (strcmp(url->scheme, "https") == 0) {
		https_content_init(sockfd, http_request);
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
	int buf_size = 128;
	int response_size = buf_size * 4;
	int len = 0;
	int header_length = 0;
	bool is_body = false;
	http_response_header *response_header;

	char response_buf[buf_size];
	memset(response_buf, 0, buf_size);

	char *response = (char *)malloc(response_size);
	memset(response, 0, response_size);

	download_filename = filename;
	int fd = open(filename, O_CREAT | O_WRONLY, 0777);

	while ((len = SSL_read(ssl, response_buf, buf_size)) > 0) {

		if (is_body) {
			write(fd, response_buf, len);
			download_size += len;
			download_progress(download_size,
					  response_header->content_length);
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

		response_header =
		    http_response_header_prase(response, header_length);

		char *response_body = response + header_length + 4;

		download_size = strlen(response_body);
		download_start_ms = get_time_ms();
		write(fd, response_body, download_size);

		int response_buf_len = strlen(response_buf);

		if (response_buf_len < len) {
			int padding_size = len - response_buf_len;
			char *padding = response_buf + response_buf_len;
			write(fd, padding, padding_size);
			download_size += padding_size;
		}

		download_progress(download_size,
				  response_header->content_length);
	}

	download_end();
	free(response);

	SSL_shutdown(ssl);
	SSL_free(ssl);

	close(sockfd);

	SSL_CTX_free(ctx);

	return 0;
}

void download_end()
{
	long times = get_time_ms() - download_start_ms;
	char *date_time = get_date_time();

    printf("\n\n%s (%0.2f KB/s) - 已保存 \"%s\"\n\n", date_time,
           download_size / (((float)times / 1000) * 1024),
           download_filename);

	char *progress_format = get_progress_format();
	free(progress_format);
	free(date_time);
}
