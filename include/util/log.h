#ifndef FUDGE_LOG_H_
#define FUDGE_LOG_H_

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

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

#if LOG_LEVEL <= 2
#define WARN(...) do { \
  fprintf(stdout, "[W] "); \
  fprintf(stdout, __VA_ARGS__); \
  fprintf(stdout,"\n"); \
} while (0)
#else
#define WARN(...) ((void)0)
#endif

#if LOG_LEVEL <= 3
#define ERROR(...) do { \
  fprintf(stdout, "[E] "); \
  fprintf(stdout, __VA_ARGS__); \
  fprintf(stdout,"\n"); \
} while (0)
#else
#define ERROR(...) ((void)0)
#endif

#endif /* FUDGE_LOG_H_ */
