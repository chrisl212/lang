/* Created by Chris Loonam on 11/25/15 */

#include <stdarg.h>
#include <stdlib.h>
#include "err.h"

void error(int code, const char *fmt, ...) {
	char *str;
	va_list lst;

	va_start(lst, fmt);

	vasprintf(&str, fmt, lst);

	va_end(lst);
	fprintf(stderr, "Parser error %d : %s\n", code, str);
	exit(code);
}
