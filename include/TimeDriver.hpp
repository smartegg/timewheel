#ifndef TIME_DRIVER_HPP
#define TIME_DRIVER_HPP


#include <vector>
#include "TimeWheel.hpp"
#include "Timer.hpp"

namespace NDSL {

/**
 * @brief 
 *    this class is  the interface for the user to use time driver.
 *
 */
class TimeDriver {
 public:
  enum {
    DEFAULT_GRANULARITY =  100
  };
  typedef std::tr1::shared_ptr<TimeWheel> TimeWheelPtr;

  explicit TimeDriver(long granularity = DEFAULT_GRANULARITY) ;

  virtual ~TimeDriver();

  //The timewheel here mounted  please be multiple of this driver's granularity. 
  //or will not be accurated.
  void mountTimeWheel(std::tr1::shared_ptr<TimeWheel> timewheel) ;
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

}
#endif
