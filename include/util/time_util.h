#ifndef FUDGE_TIME_UTIL_H_
#define FUDGE_TIME_UTIL_H_

#include <chrono>
#include <iostream>

#define GET_TIME(x) do { \
  x = std::chrono::duration_cast<std::chrono::milliseconds>( \
      std::chrono::system_clock::now().time_since_epoch()).count(); \
} while (0)

#define TIME_TYPE std::chrono::milliseconds::rep

#define PREPARE_TIMER TIME_TYPE start_time, end_time;

#define START_TIMER GET_TIME(start_time);

#define END_TIMER GET_TIME(end_time);

#define PRINT_TIME_ELAPSED \
  std::cout << "Time elapsed: " << end_time - start_time \
      << "ms" << std::endl; \

#endif /* FUDGE_TIME_UTIL_H_ */
