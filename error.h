#ifndef _PERROR_H_
#define _PERROR_H_
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define PERROR(...) \
    _error(__FILE__, __FUNCTION__, __LINE__,  __VA_ARGS__);

__declspec(dllexport) void _error(const char *file, const char *function, uint16_t line, const char *fmt,...);


#endif // _PERROR_H_
