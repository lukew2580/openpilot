#ifndef _LINUX_TIME_H
#define _LINUX_TIME_H

#include <linux/types.h>

typedef __s64 __kernel_time_t;
typedef __s64 __kernel_suseconds_t;

struct timeval {
	__kernel_time_t		tv_sec;
	__kernel_suseconds_t	tv_usec;
};

#endif /* _LINUX_TIME_H */