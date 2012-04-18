#ifndef NDSL_TIME_DRIVER_BY_SELECT_H
#define NDSL_TIME_DRIVER_BY_SELECT_H

#include "TimeDriver.hpp"

namespace NDSL {


/**
 * @brief  use select() method to make a timer
 */

class TimeDriverBySelect : public TimeDriver{
 public:
  explicit TimeDriverBySelect(long granularity = DEFAULT_GRANULARITY);
  virtual ~TimeDriverBySelect();
  virtual  int start();
};

}

#endif
