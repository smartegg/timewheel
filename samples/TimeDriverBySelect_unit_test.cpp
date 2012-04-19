#include <tr1/memory>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "TimerUtil.hpp"


using namespace NDSL;

class Job1 : public BaseJob {
 public:
  virtual int run(Timer* timer) {
    puts("Job1 done");
    return 0;
  }
};

class Job2 : public BaseJob {
 public:
  virtual int run(Timer* timer) {
    puts("Job2 done");
    //cancel this repeated timer.
    timer->getTimeWheel()->stopTimer(timer);
    return 0;
  }
};

class Job0 : public BaseJob {
 public:
  virtual int run(Timer* timer) {
    puts("Job0 done");
    //cancel this repeated timer.
    timer->getTimeWheel()->stopTimer(timer);
    return 0;
  }
};


class Job3 : public BaseJob {
 public:
  Job3() : BaseJob() ,
           cnt_(0) {

  }
  virtual int run(Timer* timer) {
    puts("job3 invoke");
    cnt_++;
    std::tr1::shared_ptr<BaseJob> job0(new Job0());
    std::tr1::shared_ptr<Timer> timer0(new Timer(job0, cnt_ *  100, false));
    timer->getTimeWheel()->addTimer(timer0);
    return 0;
  }
 private:
  int cnt_ ;
};


int main() {
  using namespace std::tr1;
  shared_ptr<TimeDriver> timedriver(new TimeDriverBySelect());
  shared_ptr<TimeWheel> timewheel(new TimeWheel());

  shared_ptr<BaseJob> job0(new Job0());//job0 done
  shared_ptr<BaseJob> job1(new Job1());//job1 done
  shared_ptr<BaseJob> job2(new Job2());//after 500, stop
  shared_ptr<BaseJob> job3(new Job3());//after 300ms, create a 100ms job0, after 600ms, create a 200ms job0, after 900ms, creat job3
  shared_ptr<Timer> timer0(new Timer(job0, 100,false));
  shared_ptr<Timer> timer1(new Timer(job1, 200,false));
  shared_ptr<Timer> timer2(new Timer(job2, 500,true));
  shared_ptr<Timer> timer3(new Timer(job3, 300,true));

  timewheel->addTimer(timer0);
  timewheel->addTimer(timer1);
  timewheel->addTimer(timer2);
  timewheel->addTimer(timer3);

  timedriver->mountTimeWheel(timewheel);
  timedriver->start();

  return 0;
}
