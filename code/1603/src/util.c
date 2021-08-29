#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "util.h"

time_t get_time_ts()
{
	return time(NULL);
}

long get_time_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

char *get_date_time()
{

	struct tm *time_info;
	time_t ts = get_time_ts();
	time_info = localtime(&ts);

	int len = strlen("1970-01-01 00:00:00") + 1;
	char buffer[len];
	memset(buffer, 0, len);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);

	return strndup(buffer, len - 1);
}
