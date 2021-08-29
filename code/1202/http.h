#ifndef MYWGT_HTTP_H
#define MYWGT_HTTP_H

#include "parse.h"

char *http_create_request(http_url * url);
int http_connect(http_url * url);
int http_download(int sockfd);

#endif
