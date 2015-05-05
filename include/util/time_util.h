#ifndef TIME_UTIL_H_
#define TIME_UTIL_H_

#ifdef _WIN32
#include <sys/timeb.h>
#define GET_TIME(x) do { \
  ftime(&(x)); \
} while (0)
#else
#include <sys/time.h>
#define GET_TIME(x) do { \
  gettimeofday(&(x), NULL); \
} while (0)
#endif

#ifdef _WIN32
#define TIME_TYPE  timeb
#else
#define TIME_TYPE  struct timeval
#endif

#ifdef _WIN32
#define TIME_DELTA(end, start) \
  ((int)((end.time - start.time)*1000 + end.millitm - start.millitm))
#else
#define TIME_DELTA(end, start) \
  ((int)((end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000))
#endif

#define PREPARE_TIMER \
  TIME_TYPE start_time, end_time;

#define START_TIMER \
  GET_TIME(start_time);

#define END_TIMER \
  GET_TIME(end_time);

#define PRINT_TIME_ELAPSED \
  printf("Time elapsed: %d ms\n", TIME_DELTA(end_time, start_time));

#endif /* TIME_UTIL_H_ */
