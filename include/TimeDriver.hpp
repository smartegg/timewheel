/**
 * @file TimeDriver.hpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 16:55:16
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
#ifndef _TIME_DRIVER_HPP_
#define _TIME_DRIVER_HPP_


#include <vector>
#include <tr1/memory>
#include "TimeWheelInterface.hpp"

namespace ndsl {

/**
 * @brief
 *    this class is  the interface for the user to use time driver.
 *
 */
class TimeDriver {
  public:
    enum {
      DEFAULT_GRANULARITY =  50
    };
    typedef std::tr1::shared_ptr<TimeWheelInterface> TimeWheelPtr;

    explicit TimeDriver(long granularity = DEFAULT_GRANULARITY) ;

    virtual ~TimeDriver();

    //The timewheel here mounted  please be multiple of this driver's granularity.
    //or will not be accurated.
    void mountTimeWheel(TimeWheelPtr timewheel) ;
    virtual int start() = 0;

    int totalWheels() const;
    long getGranularity() const;

  protected:
    //should be called by inner timer, not user.
    //the semantic is : called every @granularity_ milliseconds.
    //after called this , @ticks_++
    int tick();


  protected:

    long granularity_;
    int ticks_ ;
    std::vector<TimeWheelPtr> wheels_;

  private:
    //forbid copy and assignment semantic
    TimeDriver(const TimeDriver&);
    TimeDriver& operator= (const TimeDriver&);
};

inline long TimeDriver::getGranularity() const {
  return granularity_;
}

}//namespace ndsl
#endif // _TIME_DRIVER_HPP_
