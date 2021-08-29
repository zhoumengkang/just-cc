#ifndef MYWGT_PARSE_H
#define MYWGT_PARSE_H

char *parse_scheme(const char *url);
char *parse_domain(const char *url, const char *scheme);
char *parse_host(const char *domain);
int parse_port(const char *host, const char *scheme);

#endif
