#include "TimeDriver.hpp"

#include "TimeHelper.hpp"

namespace ndsl {

TimeDriver::TimeDriver(long granularity)
  : granularity_(granularity),
    ticks_(0) {
}

TimeDriver::~TimeDriver() {

}

void TimeDriver::mountTimeWheel(TimeDriver::TimeWheelPtr timewheel) {
  wheels_.push_back(timewheel);
}

int TimeDriver::tick() {
  ticks_ ++;

  for (size_t i = 0; i < wheels_.size(); i++) {
    int ntimes = wheels_[i]->getGranularity() / granularity_;

    if (ntimes == 0) {
      ntimes++;
    }

    if (ticks_  % ntimes  == 0) {
#ifdef TIME_DRIVER_STATUS
      puts("timedriver tick");
#endif
      wheels_[i]->run(granularity_);
    }
  }

  return 0;
}

int TimeDriver::totalWheels() const {
  return wheels_.size();
}


}
