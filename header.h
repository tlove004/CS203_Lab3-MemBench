#ifndef __headers__
#define __headers__


#define gettime     usecs_spent
#define TRIES 11

#define NULL 0

#include <stdint.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <float.h>
# include <limits.h>
# include <sys/time.h>

#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <rpc/rpc.h>
#include <rpc/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include "timing.h"

#define	LONGER		 7500000	/* for networking data transfers */


typedef struct {
    uint64_t u;
    uint64_t n;
} value_t;
typedef struct {
     int N;
     value_t v[TRIES];
} result_t;

int sizeof_result(int N);
result_t* get_results();

#define SHORT        1000000
typedef long iter_t;

typedef void (*benchmp_f)(iter_t iterations, void* cookie);

extern void insertsort(uint64_t, uint64_t, result_t *);

extern void insertinit(result_t*);

extern void mem_cleanup(iter_t, void*);

extern void set_results(result_t*);

extern int handle_scheduler(int childno, int benchproc, int nbenchprocs);


#define	BENCHO(loop_body, overhead_body, enough) { 			\
	int 		__i, __N;					\
	double 		__oh;						\
	result_t 	__overhead, __r;				\
	insertinit(&__overhead); insertinit(&__r);			\
	__N = (enough == 0 || get_enough(enough) <= 100000) ? TRIES : 1;\
	if (enough < LONGER) {loop_body;} /* warm the cache */		\
	for (__i = 0; __i < __N; ++__i) {				\
		BENCH1(overhead_body, enough);				\
		if (gettime() > 0)					\
			insertsort(gettime(), get_n(), &__overhead);	\
		BENCH1(loop_body, enough);				\
		if (gettime() > 0)					\
			insertsort(gettime(), get_n(), &__r);		\
	}								\
	for (__i = 0; __i < __r.N; ++__i) {				\
		__oh = __overhead.v[__i].u / (double)__overhead.v[__i].n; \
		if (__r.v[__i].u > (uint64)((double)__r.v[__i].n * __oh)) \
			__r.v[__i].u -= (uint64_t)((double)__r.v[__i].n * __oh);	\
		else							\
			__r.v[__i].u = 0;				\
	}								\
	*(get_results()) = __r;						\
}

#define	BENCH(loop_body, enough) { 					\
	long		__i, __N;					\
	result_t 	__r;						\
	insertinit(&__r);						\
	__N = (enough == 0 || get_enough(enough) <= 100000) ? TRIES : 1;\
	if (enough < LONGER) {loop_body;} /* warm the cache */		\
	for (__i = 0; __i < __N; ++__i) {				\
		BENCH1(loop_body, enough);				\
		if (gettime() > 0)					\
			insertsort(gettime(), get_n(), &__r);		\
	}								\
	*(get_results()) = __r;						\
}

#define	BENCH1(loop_body, enough) { 					\
	double		__usecs;					\
	BENCH_INNER(loop_body, enough);  				\
	__usecs = gettime();						\
	__usecs -= t_overhead() + get_n() * l_overhead();		\
	settime(__usecs >= 0. ? (uint64_t)__usecs : 0);			\
}
	
#define	BENCH_INNER(loop_body, enough) { 				\
	static iter_t	__iterations = 1;				\
	int		__enough = get_enough(enough);			\
	iter_t		__n;						\
	double		__result = 0.;					\
									\
	while(__result < 0.95 * __enough) {				\
		start(0);						\
		for (__n = __iterations; __n > 0; __n--) {		\
			loop_body;					\
		}							\
		__result = stop(0,0);					\
		if (__result < 0.99 * __enough 				\
		    || __result > 1.2 * __enough) {			\
			if (__result > 150.) {				\
				double	tmp = __iterations / __result;	\
				tmp *= 1.1 * __enough;			\
				__iterations = (iter_t)(tmp + 1);	\
			} else {					\
				if (__iterations > (iter_t)1<<27) {	\
					__result = 0.;			\
					break;				\
				}					\
				__iterations <<= 3;			\
			}						\
		}							\
	} /* while */							\
	save_n((uint64_t)__iterations); settime((uint64_t)__result);	\
}


#endif /* __headers__ */
