#include "TimeWheel.hpp"

#include <cassert>
namespace {
   //this is a helper class to help auto delete the array.
   template <typename T>
   struct my_array_deleter {
     void operator()(T* p) {
       delete[] p;
     }
   };
}

namespace NDSL {

TimeWheel::TimeWheel(long granularity, int wheelSize) 
    : spokewheel_(new Spoke[wheelSize],
                  my_array_deleter<Spoke>()),
      granularity_(granularity) ,
      currentIndex_(wheelSize-1) ,
      wheelSize_(wheelSize){

}

TimeWheel::~TimeWheel() {

}

int TimeWheel::totalTimers() const {
  Spoke*  spokes = spokewheel_.get();
  int nc = 0;
  for(int i = 0;i <  wheelSize_ ; i++)
    nc += spokes[i].size();
  return nc;
}
TimeWheel::TimerId TimeWheel::addTimer(std::tr1::shared_ptr<Timer> timer) {
                          
  assert(currentIndex_ < wheelSize_);

  Spoke*  spokes = spokewheel_.get();
  int nc = timer->getInterval() / granularity_;
  int rotationtimes = nc / wheelSize_;
  int lest = nc % wheelSize_ ;
  if (lest == 0)
    lest++;

  DataType data = DataType(rotationtimes, timer);
  int offset = (currentIndex_ + lest) % wheelSize_;

  spokes[offset].push_front(data);
  return TimerId(offset, spokes[offset].begin());
}

int TimeWheel::stopTimer(TimeWheel::TimerId timerid){
  int offset = timerid.first;
  Spoke::iterator iter = timerid.second;
  Spoke*  spokes = spokewheel_.get();
  if (offset >=0 && offset < wheelSize_)
    spokes[offset].erase(iter);
  else 
    return -1;
  return 0; 
}

int TimeWheel::expiryProcessing(std::tr1::shared_ptr<Timer> timer) const {
  timer->expiryAction(timer.get()); 
  return 0;
}

int TimeWheel::perTickBookKeeping() {
  currentIndex_++;
  currentIndex_ %= wheelSize_;

  Spoke*  spokes = spokewheel_.get();
  Spoke list =  spokes[currentIndex_];
  
  for(Spoke::iterator iter = list.begin();
      iter != list.end();
      ) {
    if (iter->first == 0) {
      //call back 
      expiryProcessing(iter->second);

      if (iter->second->needRestart()) {
        std::tr1::shared_ptr<Timer> newTimer(
            new Timer(iter->second->getJob(),
                iter->second->getInterval(),
            true));
        iter = list.erase(iter);
        addTimer(newTimer);
      } else {
        iter = list.erase(iter);
      }
      

    } else {
      assert(iter->first > 0);
      iter->first--;
      iter++;
    }
  }
  return 0;
}
                      

}
