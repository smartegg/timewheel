#ifndef _TIMEDRIVERBYASIOTIMER_HPP_
#define _TIMEDRIVERBYASIOTIMER_HPP_

#include "TimeDriver.hpp"

namespace ndsl {


/**
 * @brief TimeDriverByAsioTimer
 *  this timedriver is driven by the asio::deadline_timer
 */
class TimeDriverByAsioTimer : public TimeDriver {
  public:
    explicit TimeDriverByAsioTimer(long granularity = DEFAULT_GRANULARITY);
    virtual ~TimeDriverByAsioTimer();

    virtual int start();
};

} //namespace ndsl


#endif // _TIMEDRIVERBYASIOTIMER_HPP_

