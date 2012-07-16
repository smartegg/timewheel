#include "TimeDriverByAsioTimer.hpp"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "TimeHelper.hpp"


namespace ndsl {




TimeDriverByAsioTimer::TimeDriverByAsioTimer(long granularity)
  : TimeDriver(granularity) {

}

TimeDriverByAsioTimer::~TimeDriverByAsioTimer() {

}

int TimeDriverByAsioTimer::start() {
  boost::asio::io_service io;
  boost::asio::deadline_timer timer(io, boost::posix_time::milliseconds(granularity_));

  printf_elapsed_time();
  for (; ;) {
    timer.wait();

    tick();
    printf_elapsed_time();
    timer.expires_from_now(boost::posix_time::milliseconds(granularity_));
  }
  return 0;
}

} //namespace ndsl
