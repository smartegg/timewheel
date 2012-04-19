#include "TimeWheel.hpp"

#include <cassert>
#include <vector>
#include "TimeHelper.hpp"

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
                  ::my_array_deleter<Spoke>()),
      granularity_(granularity) ,
      currentIndex_(0) ,
      wheelSize_(wheelSize){
  assert(granularity_ != 0);
  assert(wheelSize_ != 0);
}

TimeWheel::~TimeWheel() {

}

int TimeWheel::totalTimers() const {
  Spoke*  spokes = spokewheel_.get();
  int nc = 0;
  for(int i = 0;i <  wheelSize_ ; i++) {
    nc += spokes[i].size();
  }
  return nc;
}
void TimeWheel::addTimer(std::tr1::shared_ptr<Timer> timer) {
                          
  assert(currentIndex_ < wheelSize_);

  Spoke*  spokes = spokewheel_.get();
  int nc = timer->getInterval() / granularity_;
  if (nc == 0)
    nc++;
  int rotationtimes = nc / wheelSize_;

  DataType data = DataType(rotationtimes, timer);
  int offset = (currentIndex_ + nc) % wheelSize_;

  spokes[offset].push_front(data);
  timer->setTimerId(TimerId(offset, spokes[offset].begin()));
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
  Spoke& list =  spokes[currentIndex_];
  

  int ndeleted = 0;

  std::vector<std::tr1::shared_ptr<Timer> > newtimers;
  for(Spoke::iterator iter = list.begin();
      iter != list.end();
      ) {
    if (iter->first == 0) {
      //dispatch the BaseJob 
      expiryProcessing(iter->second);

      if (iter->second->needRestart()) {
        std::tr1::shared_ptr<Timer> newTimer(
            new Timer(iter->second->getJob(),
            iter->second->getInterval(),
            true));
        iter = list.erase(iter);
        newtimers.push_back(newTimer); 
      } else {
        iter = list.erase(iter);
        ndeleted ++;
      }

    } else {
      assert(iter->first > 0);
      iter->first--;
      iter++;
    }
  }
  //restart the repeated timer
  for (size_t i = 0; i < newtimers.size(); i++) {
    addTimer(newtimers[i]);
  }


  return ndeleted;
}

int TimeWheel::stopTimer(std::tr1::shared_ptr<Timer> timer) {
  int status = stopTimer(timer->getTimerId());
  timer->resetTimerId();
  return  status;
}

                      

}
