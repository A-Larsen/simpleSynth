#include "error.h"

void _error(const char *file, const char *function, uint16_t line, const char *fmt,...){
	va_list args;
	va_start(args, fmt);
    char newstr[500];
	vsnprintf(newstr, 500, (const char *)fmt, args);

    fprintf(stderr, 
            "\n"
			"\x1b[91mERROR \x1b[39m\n\n"
            "\x1b[92mfile\x1b[39m         %s\n"
            "\x1b[92mfunction\x1b[39m     %s\n"
            "\x1b[92mline\x1b[39m         %d\n"
            "\n%s\n\n",
            file, function,
            line, newstr);


	va_end(args);
}

