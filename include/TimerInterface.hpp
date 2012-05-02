/**
 * @file Timer.hpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 15:00:55
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */

#ifndef _NDSL_TIMER_INTERFACE_HPP_
#define _NDSL_TIMER_INTERFACE_HPP_

namespace ndsl {

class TimerInterface {
  public:
    explicit TimerInterface(int timespan, bool needRepeat) {}
    virtual ~TimerInterface() {}
    virtual void callback() = 0;
    virtual int getTimeSpan() const = 0;
    virtual void setTimeSpan(int timespan) = 0;
    virtual bool isRegistered() const = 0;
    virtual bool needRepeat() const = 0;

};

}//namespace ndsl
#endif //_NDSL_TIMER_INTERFACE_HPP_
