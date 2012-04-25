#ifndef _NDSL_TIME_WHEEL_HPP_
#define _NDSL_TIME_WHEEL_HPP_

#include <boost/intrusive/list.hpp>
#include "BasicTimeWheel.hpp"

namespace ndsl {
class TimeWheel : public BasicTimeWheel {
  public:
    typedef BasicTimeWheel::Timer Timer; 

    explicit TimeWheel(int frequence = 100, int wheelSize_ = 1024);
    virtual ~TimeWheel();

    void run(int milliseconds);

    int tick();
};

}//namespace ndsl
#endif //_NDSL_TIME_WHEEL_HPP_
