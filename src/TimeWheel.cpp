#include "TimeWheel.hpp"

#include <cassert>
#include <vector>



namespace ndsl {

using namespace boost;

TimeWheel::TimeWheel(int frequence, int wheelSize)
  : currentIndex_(0),
    wheelSize_(wheelSize) ,
    frequence_(frequence) {
  wheel_ = new Spoke[wheelSize];
}

TimeWheel::~TimeWheel() {
  delete []wheel_;
}

void TimeWheel::addTimer(TimeWheel::Timer& timer) {

  assert(currentIndex_ < wheelSize_);

  Spoke*  spokes = wheel_;

  int nc = (timer.getTimeSpan()  + frequence_ - 1) / frequence_;

  if (nc == 0) {
    nc++;
  }

  timer.rc_ = (nc + wheelSize_ - 1) / wheelSize_; //just get the ceilling value , not floor value.

  int offset = (currentIndex_ + nc) % wheelSize_;

  spokes[offset].push_front(timer);
  timer.setTimeWheel(this);
}


/**
 * @brief
 * every @frequent  ms, this will be invoked
 */

int TimeWheel::tick() {
  //FIXME: not consider the repeated-timer.
  currentIndex_++;
  currentIndex_ %= wheelSize_;

  Spoke*  spokes =  wheel_;
  Spoke& list =  spokes[currentIndex_];
  std::vector<Timer*> waits;

  for (Spoke::iterator it(list.begin()), itend(list.end());
       it != itend;
       it++)  {
    if ((*it).rc_ == 1) {
      waits.push_back(&(*it));//just record the pointer of it.
//      it = list.erase(it);
    } else {
      assert(it->rc_ > 1);
      it->rc_--;
    }
  }

  for (size_t i = 0; i < waits.size(); i++) {
    waits[i]->callback();
  }

  for (size_t i = 0; i< waits.size(); i++) {
    waits[i]->stop();
  }

  return 0;
}

size_t TimeWheel::totalTimers() const {
  size_t sum = 0;

  for (size_t i = 0; i < wheelSize_; i++) {
    sum += wheel_[i].size();
  }

  return sum;
}

void TimeWheel::run(int milliseconds) {
  int nc = (milliseconds + frequence_ -1 )/ frequence_;
  if (nc  == 0) nc++;
  for(int i = 0; i < nc; i++)
    tick();
}


}
