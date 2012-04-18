#ifndef NDSL_TIMER_H
#define NDSL_TIMER_H

#include <cstdio>
#include <tr1/memory>

namespace NDSL {
class BaseJob;
//base Timer class
class Timer {
 public:
  Timer(const std::tr1::shared_ptr<BaseJob> jobptr, size_t interval, bool needRestart) 
    :interval_(interval), 
     needRestart_(needRestart) ,
     jobptr_(jobptr) {
  }

  size_t getInterval() const {
    return interval_;
  }
  
  void setInterval(size_t interval) {
    interval_ =  interval;
  }
  bool needRestart() const {
    return needRestart_;
  }
  const std::tr1::shared_ptr<BaseJob> getJob() const {
    return jobptr_;
  }
  virtual int expiryAction(void* ptr);
 private:
  size_t interval_;
  bool needRestart_;
  std::tr1::shared_ptr<BaseJob> jobptr_;

};

}
#endif
