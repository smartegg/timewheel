#include "SingeletonTimeDriver.hpp"

#include "TimeHelper.hpp"
#include <cstdlib>
#include <cstring>

using namespace NDSL;



    


namespace NDSL {

void sigalrmhandler(int signum) {
  //for each timer driver, tick ++ 
  SingeletonTimeDriver* driver = SingeletonTimeDriver::getInstance();
  printf_elapsed_time();
  driver->tick();      
}

SingeletonTimeDriver* SingeletonTimeDriver::driver_ = 0;

SingeletonTimeDriver* SingeletonTimeDriver::getInstance() {
  if (driver_ == 0) {
    driver_ = new SingeletonTimeDriver();
  }
  return driver_;
}

SingeletonTimeDriver::SingeletonTimeDriver(long granularity) 
    : TimeDriver(granularity) {
      
    
}

SingeletonTimeDriver::~SingeletonTimeDriver() {
  delete driver_;
}

int SingeletonTimeDriver::start() {
  memset(&sa_, 0, sizeof(sa_)); 
  sa_.sa_handler = ::sigalrmhandler;
  sigaction(SIGALRM, &sa_, NULL);
  timer_.it_value.tv_sec = granularity_ / 1000;
  timer_.it_value.tv_usec = (granularity_ % 1000)* 1000;
  timer_.it_interval.tv_sec = granularity_ / 1000;
  timer_.it_interval.tv_usec = (granularity_ % 1000)* 1000;
  setitimer(ITIMER_REAL, &timer_, NULL);


  printf_elapsed_time();
  for (;;) {
    pause();//just wait for signal
    printf_elapsed_time();
  }
  return 0;
}


}
