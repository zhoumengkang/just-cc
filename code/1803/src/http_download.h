#ifndef MYWGT_HTTP_DOWNLOAD_H
#define MYWGT_HTTP_DOWNLOAD_H

#include "url_parse.h"

typedef struct {
	unsigned int http_code;
	unsigned long content_length;
	char *content_type;
} http_response_header;

char *http_create_request(http_url * url);
http_response_header *http_response_header_prase(const char *str, int len);
int http_connect(http_url * url);
void https_connect_init(int sockfd);
int http_send_request(int sockfd, http_url * url);
int http_download(int sockfd, const char *filename);
void download_end();

#endif
