/**
 * @file TimeDriverByTimeFd.cpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 17:37:55
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
#include "TimeDriverByTimeFd.hpp"

#include "TimeHelper.hpp"
#include <sys/timerfd.h>
#include <stdint.h>
#include <string.h>


#define DBG(new_time) \
{ \
  printf("left: %d:%d , interval: %d:%d\n", (int)new_time.it_value.tv_sec, \
                                              (int)new_time.it_value.tv_nsec, \
                                              (int)new_time.it_interval.tv_sec, \
                                              (int)new_time.it_interval.tv_nsec);  \
}

namespace ndsl {


TimeDriverByTimeFd::TimeDriverByTimeFd(long granularity)
    : TimeDriver(granularity),
      timerfd_(-1){
}

TimeDriverByTimeFd::~TimeDriverByTimeFd() {

}


int TimeDriverByTimeFd::start() {
  timerfd_ = timerfd_create(CLOCK_MONOTONIC, 0);
  if (timerfd_ == -1)
    handle_error(false);
  struct itimerspec new_time;
  new_time.it_value.tv_nsec = (granularity_%1000) * 1000000;//milli to nano
  new_time.it_value.tv_sec  = granularity_  / 1000;
  new_time.it_interval.tv_nsec = (granularity_%1000) * 1000000;//milli to nano
  new_time.it_interval.tv_sec = (granularity_ / 1000);

  if (timerfd_settime(timerfd_, 0, &new_time, NULL)  == -1)
    handle_error(true);

  uint64_t s;
  uint64_t expired = 0, totalexpired = 0;
  printf_elapsed_time();
  for(;;) {
    //this is a block read, wait untial block.
    s = read(timerfd_, &expired, sizeof(uint64_t));
    if (s != sizeof(uint64_t))
      handle_error(false);
    totalexpired += expired;
    printf_elapsed_time();
#ifdef TIME_DRIVER_STATUS
    printf("expired: %llu; total expired=%llu\n",
                (unsigned long long) expired,
                (unsigned long long) totalexpired);
#endif
    for (uint64_t i = 0; i < expired; i++)
       tick();
  }

  return 0;
}


}
