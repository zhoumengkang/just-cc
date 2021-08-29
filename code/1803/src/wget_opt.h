#ifndef MYWGET_WGET_OPT_H
#define MYWGET_WGET_OPT_H

#include <stdlib.h>
#include <getopt.h>

#define PROGRAM_NAME "myget"

static struct option const long_opts[] = {
	{"output-document", required_argument, NULL, 'O'},
	{"debug", no_argument, NULL, 'd'},
	{"version", no_argument, NULL, 'V'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}
};

void usage(int status);

#endif
