#include <util.h>
#include <cstdarg>
#include <cstdio>

bool verbose = false;
int eprintf(const char* fmt, ...) {
	if (verbose) {
		va_list vp;
		va_start(vp, fmt);
		return vprintf(fmt, vp);
	}
	return 0;
}