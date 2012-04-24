#include "TimeWheel.hpp"

#include <cassert>



namespace ndsl {

using namespace boost;

TimeWheel::TimeWheel(int frequent = 100, int wheelSize_ = 1024)
  : frequence_(frequence),
    wheelSize(wheelSize) ,
    currentIndex_(0) {
  wheel_ = new Spoke[wheelSize];
}

TimeWheel::~TimeWheel() {
  delete []wheel_;
}

void TimeWheel::addTimer(TimeWheel::Timer timer) {

  assert(currentIndex_ < wheelSize);

  Spoke*  spokes = wheel_;
  
  int nc = (timer->getTimeSpan() + frequent_ -1 )/ frequent;

  if (nc == 0) {
    nc++;
  }

  timer.rt_ = (nc + wheelSize_ - 1) / wheelSize_; //just get the ceilling value , not floor value.

  int offset = (currentIndex_ + nc) % wheelSize_;

  spokes[offset].push_front(timer);
}


/**
 * @brief
 * every @frequent  ms, 
 */

int TimeWheel::tick() {
  currentIndex_++;
  currentIndex_ %= wheelSize_;

  Spoke*  spokes =  wheel_;
  Spoke& list =  spokes[currentIndex_];
  typedef void (*func)();
  vector<func> waits;

  for(Spoke::iterator it(list.begin()), itend(list.end());
      it != itend; )  {
    if (it->rc_ == 1) {
      waits.push_back(it->callback());
      it = list.erase(it);
    } else {
      assert(it->rc_ > 1);
      it->rc_--;
      it++;
    }
  }

  for(int i =0; i < waits.size(); i++) {
    waits[i]();
  }

  return 0;
}


}
