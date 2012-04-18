#ifndef TIME_DRIVER_BY_TIMEFD_H
#define TIME_DRIVER_BY_TIMEFD_H

#include "TimeDriver.hpp"

namespace NDSL{

/**
 * @brief this time driver just use timefd_xxx() fucntions
 *           with epoll() to drive the timewheel
 */
class TimeDriverByTimeFd : public TimeDriver{
 public:
  explicit TimeDriverByTimeFd(long granularity = DEFAULT_GRANULARITY);
  virtual ~TimeDriverByTimeFd();
  virtual int start() ;
 private:
  int timerfd_; 
};

}
#endif
