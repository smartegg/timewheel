#include <tr1/memory>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "TimerUtil.hpp"


using namespace ndsl;

class Job1 : public AdvanceTimeWheel::Timer {
 public:
  Job1(int s, bool f) : Timer(s,f) {}
  virtual void callback() {
    puts("Job1 done");
  }
};

class Job2 : public AdvanceTimeWheel::Timer {
 public:
  Job2(int s, bool f) : Timer(s,f) {}
  virtual void callback() {
    puts("Job2 done");
    //cancel this repeated timer.
    this->stop();
  }
};

class Job0 : public AdvanceTimeWheel::Timer {
 public:
  Job0(int s, bool f) : Timer(s,f) {}
  virtual void callback() {
    puts("Job0 done");
    //cancel this repeated timer.
    this->stop();
  }
};


class Job3 : public AdvanceTimeWheel::Timer {
 public:
  Job3(int s, bool f) : Timer(s, f) ,
                        cnt_(0) {

  }
  virtual void callback() {
    puts("job3 invoke");
    cnt_++;
    AdvanceTimeWheel::Timer* timer0 = new Job0(cnt_ * 100, false);
    this->getAdvanceTimeWheel()->addTimer(*timer0);
  }
 private:
  int cnt_ ;
};


int main() {
  using namespace std::tr1;
  shared_ptr<TimeDriver> timedriver(new TimeDriverBySelect());
  shared_ptr<AdvanceTimeWheel> timewheel(new AdvanceTimeWheel());

  shared_ptr<AdvanceTimeWheel::Timer> timer0(new Job0(100,false));
  shared_ptr<AdvanceTimeWheel::Timer> timer1(new Job1(200,false));
  shared_ptr<AdvanceTimeWheel::Timer> timer2(new Job2(500,true));
  shared_ptr<AdvanceTimeWheel::Timer> timer3(new Job3(300,true));

  timewheel->addTimer(*timer0);
  timewheel->addTimer(*timer1);
  timewheel->addTimer(*timer2);
  timewheel->addTimer(*timer3);

  timedriver->mountTimeWheel(timewheel);
  timedriver->start();

  return 0;
}
