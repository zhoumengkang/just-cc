#include <stdio.h>
#include "log.h"

static bool mywget_is_debug = false;

void set_debug(bool flag)
{
	mywget_is_debug = flag;
}

bool is_debug()
{
	return mywget_is_debug;
}
