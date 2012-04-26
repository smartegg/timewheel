#include "TimerTestCases.hpp"

#include "TimeWheel.hpp"
#include <boost/test/test_tools.hpp>

using namespace ndsl;

class Job : public TimeWheel::Timer {
 public:
  Job(int s, bool n = false) : Timer(s,n){}
  void callback(){}
};

void TimerTestCases::testTimeSpan() {
  TimeWheel::Timer* timerptr = new Job(100);
  TimeWheel::Timer& timer = *timerptr;
  BOOST_CHECK(timer.getTimeSpan() == 100);
}

void TimerTestCases::testInitialState() {
  Job timer(100);
  BOOST_CHECK(timer.isRegistered() == false);
  BOOST_CHECK(timer.needRepeat() == false);
  Job timer2(100, true);
  BOOST_CHECK(timer2.needRepeat() == true);
}
