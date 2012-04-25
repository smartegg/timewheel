#include "AdvanceTimeWheel.hpp"

#include <vector>
#include <cassert>

#define GRAN1 (1)
#define GRAN2 (256)
#define GRAN3 (64 * 256)
#define GRAN4 (256 * 64 * 64)
#define GRAN5 (256 * 64 * 64 * 64)

namespace ndsl {


//AdvanceTimeWheel's inner timerwheel
class InnerTimeWheel : public BasicTimeWheel {
  public:
    typedef BasicTimeWheel::Timer Timer;

    InnerTimeWheel(size_t freq, int wheelSize)
      : BasicTimeWheel(freq, wheelSize) {
    }

    void addTimer(AdvanceTimeWheel::Timer& timer) {

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




};





//below are four helper timers to keep timer to run.
//this is invoked every 256ms

class Timer0 : public AdvanceTimeWheel::Timer {
  public:
    Timer0(int s, bool f = false) : Timer(s, f) {
    }

    void callback() {
      this->timeHook_.unlink();
      this->getAdvanceTimeWheel()->wheels_[0]->addTimer(*this);
      //just simulate repeat, we need insert to a special wheel, not a wheel
      //dispatched by AdvanceTimeWheel
      this->getAdvanceTimeWheel()->move_timers(1);
    }
    //make stop useless
    void stop() {}
};

class Timer1 : public AdvanceTimeWheel::Timer {
  public:
    Timer1(int s, bool f = false) : Timer(s, f) {
    }
    void callback() {
      this->timeHook_.unlink();
      this->getAdvanceTimeWheel()->wheels_[1]->addTimer(*this);
      this->getAdvanceTimeWheel()->move_timers(2);
    }
    void stop() {}
};

class Timer2 : public AdvanceTimeWheel::Timer {
  public:
    Timer2(int s, bool f = false) : Timer(s, f) {
    }
    void callback() {
      this->timeHook_.unlink();
      this->getAdvanceTimeWheel()->wheels_[2]->addTimer(*this);
      this->getAdvanceTimeWheel()->move_timers(3);
    }
    void stop() {}
};

class Timer3 : public AdvanceTimeWheel::Timer {
  public:
    Timer3(int s, bool f = false) : Timer(s, f) {
    }
    void callback() {
      this->timeHook_.unlink();
      this->getAdvanceTimeWheel()->wheels_[3]->addTimer(*this);
      this->getAdvanceTimeWheel()->move_timers(4);
    }
    void stop() {}
};



AdvanceTimeWheel::AdvanceTimeWheel() {
  wheels_[0] = new InnerTimeWheel(1, 256);
  wheels_[1] = new InnerTimeWheel(256, 64);
  wheels_[2] = new InnerTimeWheel(256 * 64, 64);
  wheels_[3] = new InnerTimeWheel(256 * 64 * 64, 64);
  wheels_[4] = new InnerTimeWheel(256LL * 64 * 64 * 64, 64);


  timers_[0] = new  Timer0(256 , false);
  wheels_[0]->addTimer(*timers_[0]);
  

  timers_[1] = new  Timer1(256 * 64 , false);
  wheels_[1]->addTimer(*timers_[1]);

  timers_[2] = new  Timer2(256 * 64 * 64, false);
  wheels_[2]->addTimer(*timers_[2]);

  timers_[3] = new  Timer3(256LL * 64 * 64 * 64, false);
  wheels_[3]->addTimer(*timers_[3]);

  for(int i = 0; i < 4; i++)
    timers_[i]->setAdvanceTimeWheel(this);
}

AdvanceTimeWheel::~AdvanceTimeWheel() {
  for (int i = 0; i < 5; i++) {
    delete wheels_[i];
  }

  for (int i = 0; i < 4; i++) {
    delete timers_[i];
  }
}


void AdvanceTimeWheel::run(int milliseconds) {
  for (int i = 0; i < milliseconds; i++) {
    tick();
  }
}

void AdvanceTimeWheel::addTimer(AdvanceTimeWheel::Timer& timer) {
  //FIXME: if the timer is 0ms, just run it now.
  timer.stop();
  size_t sum =  0; 
  timer.left[4] = timer.getTimeSpan() / GRAN5;
  sum  = timer.left[4] * GRAN5;
  timer.left[3] = (timer.getTimeSpan()  - sum) / GRAN4;
  sum += timer.left[3] * GRAN4;
  timer.left[2] = (timer.getTimeSpan() - sum)/ GRAN3;
  sum += timer.left[2] * GRAN3;
  timer.left[1] = (timer.getTimeSpan() - sum) / GRAN2;
  sum += timer.left[1] * GRAN2;
  timer.left[0] = (timer.getTimeSpan() - sum )  / GRAN1;


  timer.preId = -1;

  for (int i = 4; i >= 0; i--) {
    if (timer.left[i] != 0) {
      if(timer.getTimeSpan() == 600)
        printf("put %d %d\n", timer.getTimeSpan(), i + 1);
      
      timer.setTimeSpan(timer.left[i]);
      wheels_[i]->addTimer(timer);
      timer.setAdvanceTimeWheel(this);

      for (int j = i - 1; j >= 0; j--) {
        if (timer.left[j] != 0) {
          timer.preId = j;
          break;
        }
      }
      break;
    }
  }
}

size_t AdvanceTimeWheel::totalTimers() const {
  size_t sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += wheels_[i]->totalTimers();
  }

  return sum - 4;// because we use four helper timer to keep our data-structure.
}


void AdvanceTimeWheel::tick() {
  std::vector<BasicTimeWheel::Timer*> timers = wheels_[0]->tick(); 
  for (size_t i = 0; i < timers.size(); i++) {
    timers[i]->callback();
  }

  for (size_t i = 0; i < timers.size(); i++) {
    timers[i]->stop();
    if(timers[i]->needRepeat()) {
      Timer* timer = dynamic_cast<Timer*>(timers[i]);
      assert(timer != 0);
      addTimer(*timer);
      timer->setAdvanceTimeWheel(this);
    } 
  }
}

//make this wheel tick , and move the expired timers  to the smaller wheel
//precondition: the timers are all not-expired
//              && wheelId >=0 && < 5
void AdvanceTimeWheel::move_timers(int wheelId) {
  using namespace std;
  assert(0);
  assert(wheelId >= 0 && wheelId < 5);

  vector<BasicTimeWheel::Timer*>& timers = wheels_[wheelId]->tick();
  vector<Timer*> delay;

  for (size_t i = 0; i < timers.size(); i++) {
    Timer* timer = dynamic_cast<Timer*>(timers[i]);
    assert(timer != 0);

    int preid = timer->preId;

    if (preid != -1)  {
      timer->stop();
      timer->setAdvanceTimeWheel(this);
      timer->setTimeSpan(timer->left[preid]);
      wheels_[preid]->addTimer(*timer);

      timer->preId = -1;
      for (int j = preid - 1; j >= 0; j--) {
        if (timer->left[j] != 0) {
          timer->preId = j;
          break;
        }
      }
      
    } else {
      //expired callback
      delay.push_back(timer);
    }
  }

  for (size_t i = 0; i < delay.size(); i++) {
    delay[i]->callback();
  }

  for (size_t i = 0; i < delay.size(); i++) {
    delay[i]->stop();
    if(delay[i]->needRepeat()) {
      addTimer(*delay[i]);
      delay[i]->setAdvanceTimeWheel(this);
    }
  }
}

}//namespace ndsl
