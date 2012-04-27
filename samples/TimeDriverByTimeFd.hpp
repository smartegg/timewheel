#ifndef _TIME_DRIVER_BY_TIMEFD_HPP_
#define _TIME_DRIVER_BY_TIMEFD_HPP_

#include "TimeDriver.hpp"

namespace ndsl {

/**
 * @brief this time driver just use timefd_xxx() fucntions
 *           with epoll() to drive the timewheel
 */
class TimeDriverByTimeFd : public TimeDriver {
  public:
    explicit TimeDriverByTimeFd(long granularity = DEFAULT_GRANULARITY);
    virtual ~TimeDriverByTimeFd();
    virtual int start() ;
  private:
    int timerfd_;
};

} //namespace ndsl
#endif //_TIME_DRIVER_BY_TIMEFD_HPP_
