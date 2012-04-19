#ifndef NDSL_TIME_WHEEL_H
#define NDSL_TIME_WHEEL_H

#include <tr1/memory>
#include <list>
#include "Timer.hpp"

namespace NDSL {


class Timer;

class TimeWheel {
   //rotationtimes with Time*
   typedef std::pair<int, std::tr1::shared_ptr<Timer> > DataType;
   typedef std::pair<int, std::list<DataType>::iterator> TimerId;
  public:
   //the granulary is in milliseconds.
   TimeWheel(long granulary = 100, int wheelSize = 256 * 4);
   virtual ~TimeWheel();

   //register  a  timer
   void addTimer(std::tr1::shared_ptr<Timer> timer);
   //stop a running timer
   int stopTimer(std::tr1::shared_ptr<Timer> timer);
   //user has responssibility to delete timer's memory.
   int stopTimer(Timer* timer);
   
   int expiryProcessing(std::tr1::shared_ptr<Timer> timer) const;
   //invoked in every  @granulary millisecnods.  should not called by user.
   int perTickBookKeeping();
   int totalTimers() const;   
   size_t getGranularity() const {
     return granularity_;
   }

  private:
   int stopTimer(TimerId id);

   typedef std::list<DataType> Spoke;
   typedef std::tr1::shared_ptr<Spoke> SpokeWheelPtr;

   SpokeWheelPtr spokewheel_;
   long granularity_;
   int currentIndex_;
   int wheelSize_;

   bool isLegal(TimerId id) const;
  
   //forbid  copy semantic
   TimeWheel(const TimeWheel&);
   TimeWheel& operator=(const TimeWheel& timewheel);
};

}
#endif  
