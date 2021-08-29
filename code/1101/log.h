#ifndef MYWGT_LOG_H
#define MYWGT_LOG_H

#include <stdio.h>
#include <stdbool.h>

#define debug_log(format, ...) \
	do { \
		if (is_debug()) { \
			printf("\033[32m%s:%05d:\t" format "\033[0m", __FILE__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (0);

#define info_log(format, ...) \
    do { \
        printf("\033[31m" format "\033[0m", ##__VA_ARGS__); \
    } while (0);

#define error_log(format, ...) \
    do { \
        printf("\033[31m" format "\033[0m", ##__VA_ARGS__); \
    } while (0);

void set_debug(bool flag);

bool is_debug();

#endif