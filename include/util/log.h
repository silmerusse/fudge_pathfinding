#ifndef LOG_H_
#define LOG_H_

#if LOG_LEVEL == 0
#define DEBUG(...) do { \
  fprintf(stdout, "[D] "); \
  fprintf(stdout, __VA_ARGS__); \
  fprintf(stdout,"\n"); \
} while (0)
#else
#define DEBUG(...) ((void)0)
#endif

#if LOG_LEVEL <= 1
#define INFO(...) do { \
  fprintf(stdout, "[I] "); \
  fprintf(stdout, __VA_ARGS__); \
  fprintf(stdout,"\n"); \
} while (0)
#else
#define INFO(...) ((void)0)
#endif

#endif /* LOG_H_ */
