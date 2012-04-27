#ifndef _NDSL_TIME_DRIVER_BY_SELECT_HPP_
#define _NDSL_TIME_DRIVER_BY_SELECT_HPP_

#include "TimeDriver.hpp"

namespace ndsl {


/**
 * @brief  use select() method to make a timer
 */

class TimeDriverBySelect : public TimeDriver {
  public:
    explicit TimeDriverBySelect(long granularity = DEFAULT_GRANULARITY);
    virtual ~TimeDriverBySelect();
    virtual  int start();
};

}//namespace ndsl

#endif// _NDSL_TIME_DRIVER_BY_SELECT_HPP_
