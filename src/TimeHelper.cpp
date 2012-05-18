/**
 * @file TimeHelper.cpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 17:05:47
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
#include "TimeHelper.hpp"


#include <string>
#include <sstream>
#include "HandleError.hpp"




namespace ndsl {


void start_count_time(void) {
#ifdef DEBUG
  printf_elapsed_time();
#endif
}

void printf_elapsed_time(void) {
#ifdef DEBUG
  static struct timespec start;
  struct timespec curr;
  static int first_call = 1;
  int secs, nsecs;

  if (first_call) {
    first_call = 0;
    int r = clock_gettime(CLOCK_MONOTONIC, &start);
    NDSL_ASSERT(r);
  }

  int r = clock_gettime(CLOCK_MONOTONIC, &curr);
  NDSL_ASSERT(r);

  secs = curr.tv_sec - start.tv_sec;
  nsecs = curr.tv_nsec - start.tv_nsec;

  if (nsecs < 0) {
    secs--;
    nsecs += 1000000000;
  }

  printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
  puts("");
#endif
}

}
