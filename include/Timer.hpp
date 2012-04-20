#ifndef NDSL_TIMER_H
#define NDSL_TIMER_H

#include <cstdio>
#include <list>
#include <tr1/memory>
#include "TimeWheel.hpp"

namespace NDSL {
class BaseJob;
class TimeWheel;

//base Timer class
class Timer {
 public:
  friend class TimeWheel;
  
  Timer(const std::tr1::shared_ptr<BaseJob> jobptr, size_t interval, bool needRestart) 
    :interval_(interval), 
     needRestart_(needRestart) ,
     jobptr_(jobptr),
     wheel_(0) {

     resetTimerId();

  }


  size_t getInterval() const {
    return interval_;
  }
  
  void setInterval(size_t interval) {
    interval_ =  interval;
  }


  TimeWheel* getTimeWheel() {
    return wheel_;
  }

  bool needRestart() const {
    return needRestart_;
  }

  void setRestart(bool needRestart) {
    needRestart_ = needRestart;
  }

  const std::tr1::shared_ptr<BaseJob> getJob() const {
    return jobptr_;
  }
 protected:
  virtual int expiryAction(void* ptr);
 private:
   typedef std::pair<int, std::tr1::shared_ptr<Timer> > DataType;
   typedef std::pair<int, std::list<DataType>::iterator> TimerId;
  

  //these func is invoked only by TimeWheel
  void setTimeWheel(TimeWheel* wheel) {
    wheel_ = wheel;
  }

  TimerId getTimerId() {
    return id_;
  }

  void setTimerId(TimerId id) {
    id_ = id;
  }

  void resetTimerId() {
    id_.first = -1;
  }

  size_t interval_;
  bool needRestart_;
  std::tr1::shared_ptr<BaseJob> jobptr_;
  TimeWheel* wheel_;

  TimerId  id_;// this variable store the timeid returned by TimeWheeel, convenient for delele  this timer in constant time.

};

}
#endif
