#include "AdvanceTimeWheel.cpp"

namespace ndsl {


//follow are four helper timers.
//
//this is invoked every 256ms
class Timer0 : public TimeWheel::Timer {
 public:
  Timer0(int s, bool f = false) : Timer(s, f){

  }

  void callback() {
    this->getTimeWheel()->wheels_[1]->tick();
  }
}

class Timer1 : public TimeWheel::Timer {
 public:
  Timer0(int s, bool f = false) : Timer(s, f){

  }

  void callback() {
    this->getTimeWheel()->wheels_[2]->tick();
  }
}

class Timer2 : public TimeWheel::Timer {
 public:
  Timer0(int s, bool f = false) : Timer(s, f){

  }

  void callback() {
    this->getTimeWheel()->wheels_[3]->tick();
  }
}

class Timer3 : public TimeWheel::Timer {
 public:
  Timer0(int s, bool f = false) : Timer(s, f){

  }

  void callback() {
    this->getTimeWheel()->wheels_[4]->tick();
  }
}









AdvanceTimeWheel::AdvanceTimeWheel() {
  wheels_[0] = new TimeWheel(1, 256);
  wheels_[1] = new TimeWheel(256, 64);
  wheels_[2] = new TimeWheel(256 * 64, 64);
  wheels_[3] = new TimeWheel(256 * 64 * 64, 64);
  wheels_[4] = new TimeWheel(256 * 64 * 64 * 64, 64);

  
  timers_[0] = new  Timer0(256 , true);
  wheels_[0].addTimer(*timers[0]);

  timers_[1] = new  Timer1(256 * 64 , true);
  wheels_[1].addTimer(*timers[1]);

  timers_[2] = new  Timer1(256 * 64 * 64, true);
  wheels_[2].addTimer(*timers[2]);
  
  timers_[3] = new  Timer1(256 * 64 * 64 * 64, true);
  wheels_[3].addTimer(*timers[3]);
}

AdvanceTimeWheel::~AdvanceTimeWheel() {
  for (int i = 0; i < 5; i++) {
    delete wheels_[i];
  }
}


void AdvanceTimeWheel::run(int milliseconds) {

}

void AdvanceTimeWheel::addTimer(AdvanceTimeWheel::Timer& timer) {

}

size_t AdvanceTimeWheel::totalTimers() const {
  size_t sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += wheels_[i].totalTimers();
  }

  return sum;
}


void AdvanceTimeWheel::tick() {
}

}//namespace ndsl
