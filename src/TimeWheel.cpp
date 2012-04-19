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
  int rotationtimes = (nc + wheelSize_ - 1)/ wheelSize_;//just get the ceilling value , not floor value.

  DataType data = DataType(rotationtimes, timer);
  int offset = (currentIndex_ + nc) % wheelSize_;

  spokes[offset].push_front(data);
  timer->setTimerId(TimerId(offset, spokes[offset].begin()));
  timer->setTimeWheel(this);
}


int TimeWheel::stopTimer(TimeWheel::TimerId timerid){
  int offset = timerid.first;
  Spoke::iterator iter = timerid.second;
  Spoke*  spokes = spokewheel_.get();
  if (isLegal(timerid)) {
    spokes[offset].erase(iter);
    return 0;
  }
  else 
    return -1;
}

int TimeWheel::expiryProcessing(std::tr1::shared_ptr<Timer> timer) const {
  timer->expiryAction(timer.get()); 
  return 0;
}

/**
 * @brief
 * becase the callback BaseJob can add/remove timer , which can cause the
 * timewheel-datasture become a mess, we use a elegant algorithm to keep it
 * clean.
 * the algorithm should be curious about the data structure consistent.
 */ 
int TimeWheel::perTickBookKeeping() {
  currentIndex_++;
  currentIndex_ %= wheelSize_;

  Spoke*  spokes = spokewheel_.get();
  Spoke& list =  spokes[currentIndex_];
  

  std::vector<std::tr1::shared_ptr<Timer> > deleted_later;//record the Time to be deleled
  std::vector<Spoke::iterator> iterators; // which need to be deleted
  for (Spoke::iterator iter = list.begin();
       iter != list.end();
       iter++) {
    if (iter->first == 1) {
        deleted_later.push_back(iter->second);
        iterators.push_back(iter);
    } else {
      assert(iter->first > 1);
      iter->first--;
    }
  }

  for (size_t i = 0; i < deleted_later.size(); i++) {
    expiryProcessing(deleted_later[i]);//this may change the timewheel inner structure, iter maybe invalid (caused by add/remove)
  }

  std::vector<std::tr1::shared_ptr<Timer> > newtimers; //record the repeated timer to be inserted again.
  for (size_t i = 0; i < deleted_later.size(); i++) {
    if(!isLegal(deleted_later[i]->getTimerId()))
      continue;
    if (deleted_later[i]->needRestart()) {
      std::tr1::shared_ptr<Timer> newTimer(
                new Timer(deleted_later[i]->getJob(),
                deleted_later[i]->getInterval(),
                true));
    
      newtimers.push_back(newTimer);
    } 
    list.erase(iterators[i]);
  }

  for (size_t i = 0; i < newtimers.size(); i++) {
    addTimer(newtimers[i]);
  }


  return 0;
}

int TimeWheel::stopTimer(std::tr1::shared_ptr<Timer> timer) {
  int status = stopTimer(timer->getTimerId());
  //just make the repeated timer failed 
  timer->setRestart(false);
  timer->resetTimerId();
  timer->setTimeWheel(0);
  return  status;
}

int TimeWheel::stopTimer(Timer*  timer) {
  int status = stopTimer(timer->getTimerId());
  //just make the repeated timer failed 
  timer->setRestart(false);
  timer->resetTimerId();
  timer->setTimeWheel(0);
  return  status;
}

bool TimeWheel::isLegal(TimeWheel::TimerId id) const {
  return id.first >= 0 && id.first < wheelSize_;
}

                      

}
