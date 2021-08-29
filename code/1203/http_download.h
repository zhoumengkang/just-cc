#ifndef MYWGT_HTTP_DOWNLOAD_H
#define MYWGT_HTTP_DOWNLOAD_H

#include "url_parse.h"

char *http_create_request(http_url * url);
int http_connect(http_url * url);
int http_download(int sockfd, const char *filename);

#endif
