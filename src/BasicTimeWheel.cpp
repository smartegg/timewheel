#include "BasicTimeWheel.hpp"

#include <cassert>
#include <vector>
#include <cstdio>



namespace ndsl {

using namespace boost;

BasicTimeWheel::BasicTimeWheel(size_t frequence, int wheelSize)
  : currentIndex_(0),
    wheelSize_(wheelSize) ,
    frequence_(frequence) {
  wheel_ = new Spoke[wheelSize];
}

BasicTimeWheel::~BasicTimeWheel() {
  delete []wheel_;
}

void BasicTimeWheel::addTimer(BasicTimeWheel::Timer& timer) {

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

std::vector<BasicTimeWheel::Timer*>& BasicTimeWheel::tick() {
  static std::vector<Timer*> waits;

  currentIndex_++;
  currentIndex_ %= wheelSize_;

  Spoke*  spokes =  wheel_;
  Spoke& list =  spokes[currentIndex_];
  waits.clear();

  for (Spoke::iterator it(list.begin()), itend(list.end());
       it != itend;
       it++)  {
    if ((*it).rc_ == 1) {
      waits.push_back(&(*it));//just record the pointer of it.
    } else {
      //never reach this.
      printf("test: %d %d\n",it->getTimeSpan(), it->rc_);
      assert(0);
      assert(it->rc_ > 1);
      it->rc_--;
    }
  }


  return waits;
}

size_t BasicTimeWheel::totalTimers() const {
  size_t sum = 0;

  for (size_t i = 0; i < wheelSize_; i++) {
    sum += wheel_[i].size();
  }

  return sum;
}



}
