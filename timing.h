/*
 * $Id$
 */

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#ifndef _TIMING_H
#define _TIMING_H

char	*p64(uint64_t big);
char	*p64sz(uint64_t big);
double	Delta(void);
double	Now(void);
void	adjust(int usec);
void	bandwidth(uint64_t bytes, uint64_t times, int verbose);
uint64_t	bytes(char *s);
void	context(uint64_t xfers);
uint64_t	delta(void);
int	get_enough(int);
uint64_t	get_n(void);
void	kb(uint64_t bytes);
double	l_overhead(void);
char	last(char *s);
void	latency(uint64_t xfers, uint64_t size);
void	mb(uint64_t bytes);
void	micro(char *s, uint64_t n);
void	micromb(uint64_t mb, uint64_t n);
void	milli(char *s, uint64_t n);
void	morefds(void);
void	nano(char *s, uint64_t n);
uint64_t	now(void);
void	ptime(uint64_t n);
void	rusage(void);
void	save_n(uint64_t);
void	settime(uint64_t usecs);
void	start(struct timeval *tv);
uint64_t	stop(struct timeval *begin, struct timeval *end);
uint64_t	t_overhead(void);
double	timespent(void);
void	timing(FILE *out);
uint64_t	tvdelta(struct timeval *, struct timeval *);
void	tvsub(struct timeval *tdiff, struct timeval *t1, struct timeval *t0);
void	use_int(int result);
void	use_pointer(void *result);
uint64_t	usecs_spent(void);
void	touch(char *buf, int size);
size_t*	permutation(int max, int scale);
int	cp(char* src, char* dst, mode_t mode);
long	bread(void* src, long count);

#if defined(hpux) || defined(__hpux)
int	getpagesize();
#endif


#define BENCH_INNER(loop_body, enough) {                \
    static iter_t   __iterations = 1;               \
    int     __enough = get_enough(enough);          \
    iter_t      __n;                        \
    double      __result = 0.;                  \
                                    \
    while(__result < 0.95 * __enough) {             \
        start(0);                       \
        for (__n = __iterations; __n > 0; __n--) {      \
            loop_body;                  \
        }                           \
        __result = stop(0,0);                   \
        if (__result < 0.99 * __enough              \
            || __result > 1.2 * __enough) {         \
            if (__result > 150.) {              \
                double  tmp = __iterations / __result;  \
                tmp *= 1.1 * __enough;          \
                __iterations = (iter_t)(tmp + 1);   \
            } else {                    \
                if (__iterations > (iter_t)1<<27) { \
                    __result = 0.;          \
                    break;              \
                }                   \
                __iterations <<= 3;         \
            }                       \
        }                           \
    } /* while */                           \
    save_n((uint64_t)__iterations); settime((uint64_t)__result);    \
}


#endif /* _TIMING_H */
