#ifndef MYWGET_PROGRESS_H
#define MYWGET_PROGRESS_H

#include <stdbool.h>

char *get_progress_format();
void download_progress(unsigned long download_size, unsigned long total_size);

#endif
