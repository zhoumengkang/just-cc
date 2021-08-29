#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "progress.h"

static int cols;

char *get_progress_format()
{
	static char *progress_format = NULL;

	if (progress_format != NULL) {
		return progress_format;
	}

	progress_format = (char *)malloc(20);
	memset(progress_format, 0, 20);

	struct winsize size;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
	cols = size.ws_col;

	const char *progress = "[]100%";
	cols = cols - strlen(progress);

	char cols_str[10] = { 0 };
	sprintf(cols_str, "%d", cols);

	strcat(progress_format, "[%-");
	strcat(progress_format, cols_str);
	strcat(progress_format, "s]%d%%\r");

	return progress_format;
}

void download_progress(unsigned long download_size, unsigned long total_size)
{
	char *progress_format = get_progress_format();

	char progress_bar[cols + 1];
	memset(progress_bar, 0, cols + 1);

	long num = (download_size * cols) / total_size;

	for (long i = 1; i <= num; i++) {
		strcat(progress_bar, "=");
	}

	printf(progress_format, progress_bar,
	       (download_size * 100) / total_size);

	fflush(stdout);
}
