#ifndef _NDSL_SINGELETON_TIME_DRIVER_HPP_
#define _NDSL_SINGELETON_TIME_DRIVER_HPP_

#include "TimeDriver.hpp"
#include <signal.h>
#include <cstddef>
#include <sys/time.h>
#include <tr1/memory>
#include <vector>


//use setitimer , only can instanse one timedriver,
//so can only use only one granularity when drive all timewheel.
//prefer the timewheel's granularity is multiple times of timedriver's granularity.

namespace ndsl {

extern void sigalrmhandler(int signum);

class SingeletonTimeDriver : public TimeDriver {
  public:
    virtual ~SingeletonTimeDriver();
    static SingeletonTimeDriver* getInstance() ;
    int start();


  protected:
    friend void ndsl::sigalrmhandler(int signum);

  private:
    explicit SingeletonTimeDriver(long granularity = TimeDriver::DEFAULT_GRANULARITY);

    //below used in setitimer
    struct itimerval timer_;
    struct sigaction sa_;
    static SingeletonTimeDriver* driver_;

    SingeletonTimeDriver(const SingeletonTimeDriver&);
    SingeletonTimeDriver& operator=(const SingeletonTimeDriver&);
};

}//namespace ndsl
#endif// _NDSL_SINGELETON_TIME_DRIVER_HPP_
