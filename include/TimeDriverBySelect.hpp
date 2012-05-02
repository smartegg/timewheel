/**
 * @file TimeDriverBySelect.hpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 17:01:33
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
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
