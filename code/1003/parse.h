#ifndef MYWGT_PARSE_H
#define MYWGT_PARSE_H

char *parse_scheme(const char *url);
char *parse_domain(const char *url, const char *scheme);

#endif
