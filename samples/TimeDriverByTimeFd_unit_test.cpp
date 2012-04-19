#include <tr1/memory>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "TimerUtil.hpp"

using namespace NDSL;

class Job1 : public BaseJob {
 public:
  virtual int run(Timer* timer) {
//    print_elapsed_time();
    puts("Job1 done");
    return 0;
  }
  
};

class Job2 : public BaseJob {
 public:
  virtual int run(Timer* timer) {
//    printf("timer: %p expired\n", timer);
//    print_elapsed_time();
    puts("Job2 done");
    return 0;
  }
  
};


int main() {
  using namespace std::tr1;
  shared_ptr<TimeDriver> timedriver(new TimeDriverByTimeFd());
  shared_ptr<TimeWheel>  timewheel(new TimeWheel());

  shared_ptr<BaseJob> job1(new Job1());
  shared_ptr<BaseJob> job2(new Job2());
  shared_ptr<BaseJob> job3(new Job1());
  //create a oneshot timer and a repeated timer.
  shared_ptr<Timer> timer1(new Timer(job1, 200,false));
  shared_ptr<Timer> timer2(new Timer(job2, 1000,true));
  shared_ptr<Timer> timer3(new Timer(job1, 1000,true));

  timewheel->addTimer(timer1);
  timewheel->addTimer(timer2);
  timewheel->addTimer(timer3);

  timedriver->mountTimeWheel(timewheel);
//  timewheel->stopTimer(id2);
  timedriver->start();


  return 0;
}
