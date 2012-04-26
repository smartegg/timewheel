#include "AdvanceTimeWheel.hpp"

#include <algorithm>
#include <vector>
#include <cassert>

#define GRAN1 (1)
#define GRAN2 (256)
#define GRAN3 (64 * 256)
#define GRAN4 (256 * 64 * 64)
#define GRAN5 (256 * 64 * 64 * 64)

namespace ndsl {
//below are four helper timers to keep timer to run.
//this is invoked every 256ms

class Timer0 : public AdvanceTimeWheel::Timer {
  public:
    Timer0(int s, bool f = false) : Timer(s, f) {
    }

    void callback() {
      //dispatched by AdvanceTimeWheel
      this->getAdvanceTimeWheel()->move_timers(1);
    }
    //a trick , just make stop useless
    //just simulate repeat, we need insert to a special wheel, not a wheel
    void stop() {}
};

class Timer1 : public AdvanceTimeWheel::Timer {
  public:
    Timer1(int s, bool f = false) : Timer(s, f) {
    }

    void callback() {
      //dispatched by AdvanceTimeWheel
      this->getAdvanceTimeWheel()->move_timers(2);
    }
    //a trick , just make stop useless
    //just simulate repeat, we need insert to a special wheel, not a wheel
    void stop() {}
};


class Timer2 : public AdvanceTimeWheel::Timer {
  public:
    Timer2(int s, bool f = false) : Timer(s, f) {
    }

    void callback() {
      //dispatched by AdvanceTimeWheel
      this->getAdvanceTimeWheel()->move_timers(3);
    }
    //a trick , just make stop useless
    //just simulate repeat, we need insert to a special wheel, not a wheel
    void stop() {}
};




class Timer3 : public AdvanceTimeWheel::Timer {
  public:
    Timer3(int s, bool f = false) : Timer(s, f) {
    }

    void callback() {
      //dispatched by AdvanceTimeWheel
      this->getAdvanceTimeWheel()->move_timers(4);
    }
    //a trick , just make stop useless
    //just simulate repeat, we need insert to a special wheel, not a wheel
    void stop() {}
};
//**  helper  classes  end **//





using namespace boost::intrusive;


AdvanceTimeWheel::Timer::Timer(int timespan, bool needRepeat)
  : timespan_(timespan),
    needRepeat_(needRepeat) ,
    wh_(0),
    inner_(0) ,
    idxlen_(0) {


  if (timespan == 0) {
    timespan_ = 1;
  }


  size_t sum =  0;
  int* left = left_;
  std::fill(left, left + 5, 0);
  left[4] = getTimeSpan() / GRAN5;
  sum  = left[4] * GRAN5;
  left[3] = (getTimeSpan()  - sum) / GRAN4;
  sum += left[3] * GRAN4;
  left[2] = (getTimeSpan() - sum) / GRAN3;
  sum += left[2] * GRAN3;
  left[1] = (getTimeSpan() - sum) / GRAN2;
  sum += left[1] * GRAN2;
  left[0] = (getTimeSpan() - sum)  / GRAN1;

  for (int i = 0; i < 5; i++)
    if (left[i] != 0) {
      idx_[idxlen_++] = i;
    }
}

void AdvanceTimeWheel::Timer::stop() {
  if (timeHook_.is_linked()) {
    timeHook_.unlink();
  }

  wh_ = 0;
  inner_ = 0;
}



class InnerTimeWheel {
  public:
    typedef AdvanceTimeWheel::Timer Timer;

    InnerTimeWheel(AdvanceTimeWheel* wh, int id, int wheelSize);
    virtual ~InnerTimeWheel();

    size_t totalTimers() const;
    size_t wheelSize() const;

    std::vector<Timer*>& tick();
    void addTimer(Timer& timer);
    AdvanceTimeWheel* getParent() const;
  protected:
    typedef list < Timer, member_hook < Timer,
            list_member_hook
            <
            link_mode<auto_unlink>
            > ,
            &Timer::timeHook_
            > ,
            constant_time_size<false>
            > Spoke;
  private:
    Spoke* wheel_;
    AdvanceTimeWheel* wh_;
    int id_;
    size_t currentIndex_;
    size_t wheelSize_;
};

inline size_t InnerTimeWheel::wheelSize() const {
  return wheelSize_;
}

inline size_t InnerTimeWheel::totalTimers()  const {
  size_t sum = 0;

  for (size_t i = 0; i < wheelSize_; i++) {
    sum += wheel_[i].size();
  }

  return sum;
}


InnerTimeWheel::InnerTimeWheel(AdvanceTimeWheel* wh,
                               int id,
                               int wheelSize)
  : wh_(wh),
    id_(id),
    currentIndex_(0),
    wheelSize_(wheelSize_ = wheelSize) {
  wheel_ = new Spoke[wheelSize];
}

InnerTimeWheel::~InnerTimeWheel() {
  delete []wheel_;
}




inline AdvanceTimeWheel* InnerTimeWheel::getParent() const {
  return wh_;
}


void InnerTimeWheel::addTimer(Timer& timer) {
  //FIXME: if the timer is 0ms, just run it now.
  assert(currentIndex_ < wheelSize_);
  assert(timer.getTimeWheel() == 0);
  assert(timer.getAdvanceTimeWheel() == 0);

  timer.setTimeWheel(this);

  assert(this->getParent() != 0);
  timer.setAdvanceTimeWheel(this->getParent());

  Spoke*  spokes = wheel_;

  assert(timer.idxlen_ - 1 >= 0);
  int nc = (timer.left_[id_]);

  if (nc == 0) {
    nc++;
  }

  timer.rc_ = (nc + wheelSize_ - 1) / wheelSize_; //just get the ceilling value , not floor value.

  assert(timer.rc_ == 1);

  int offset = (currentIndex_ + nc) % wheelSize_;

  spokes[offset].push_front(timer);
}

AdvanceTimeWheel::AdvanceTimeWheel() {
  wheels_[0]  = new InnerTimeWheel(this, 0, 256);
  wheels_[1]  = new InnerTimeWheel(this, 1, 64);
  wheels_[2]  = new InnerTimeWheel(this, 2, 64);
  wheels_[3]  = new InnerTimeWheel(this, 3, 64);
  wheels_[4]  = new InnerTimeWheel(this, 4, 64);

  timers_[0] = new  Timer0(256 , false);
  wheels_[0]->addTimer(*timers_[0]);


  timers_[1] = new  Timer1(256 * 64 , false);
  wheels_[1]->addTimer(*timers_[1]);

  timers_[2] = new  Timer2(256 * 64 * 64, false);
  wheels_[2]->addTimer(*timers_[2]);

  timers_[3] = new  Timer3(256LL * 64 * 64 * 64, false);
  wheels_[3]->addTimer(*timers_[3]);

}

AdvanceTimeWheel::~AdvanceTimeWheel() {
  for (int i = 0; i < 5; i++) {
    delete wheels_[i];
  }

  for (int i = 0; i < 4; i++) {
    delete timers_[i];
  }
}


size_t AdvanceTimeWheel::totalTimers() const {

  size_t sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += wheels_[i]->totalTimers();
  }

  return sum - 4;// because we use four helper timer to keep our data-structure.
}


void AdvanceTimeWheel::addTimer(AdvanceTimeWheel::Timer& timer) {
  //FIXME: if the timer is 0ms, just run it now.
  timer.stop();
  assert(timer.idxlen_  - 1>= 0);
  wheels_[timer.idxlen_ - 1]->addTimer(timer); 
}

void AdvanceTimeWheel::tick() {
  std::vector<AdvanceTimeWheel::Timer*> timers = wheels_[0]->tick(); 
  for (size_t i = 0; i < timers.size(); i++) {
    timers[i]->callback();
  }

  for (size_t i = 0; i < timers.size(); i++) {
    timers[i]->stop();
    if(timers[i]->needRepeat()) {
      Timer* timer = timers[i];
      assert(timer != 0);
      addTimer(*timer);
    } 
  }
}



//make this wheel tick , and move the expired timers  to the smaller wheel
//precondition: the timers are all not-expired
//              && wheelId >=0 && < 5
void AdvanceTimeWheel::move_timers(int wheelId) {
  using namespace std;
  assert(wheelId >= 0 && wheelId < 5);

  vector<AdvanceTimeWheel::Timer*>& timers =
      wheels_[wheelId]->tick();

  vector<Timer*> delay;

  typedef AdvanceTimeWheel::Timer Timer;
  for (size_t i = 0; i < timers.size(); i++) {
    Timer* timer = timers[i];
    int& len = timer->idxlen_;
    if (len > 1) {
      timer->stop();
      timer->setAdvanceTimeWheel(this);
      len--;
      wheels_[len]->addTimer(*timer);
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
    }
  }



  

}






}//namespace ndsl
