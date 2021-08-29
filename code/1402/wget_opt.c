#include <stdio.h>

#include "wget_opt.h"

void usage(int status)
{

	printf("Usage: %s [OPTION]... [URL]...\n", PROGRAM_NAME);

	if (status != 0) {
		fprintf(stderr, "Try '%s --help' for more information.\n",
			PROGRAM_NAME);
	} else {
		fprintf(stdout,
			"  -O,  --output-document=FILE  write documents to FILE\n");
		fprintf(stdout,
			"  -d,  --debug                 print debug output\n");
		fprintf(stdout,
			"  -h,  --help                  display this help and exit\n");
		fprintf(stdout,
			"  -V,  --version               output version information and exit\n");
	}

	exit(status);
}
