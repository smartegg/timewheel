#include <boost/test/unit_test.hpp>
#include "TimeWheel.hpp"

using namespace ndsl;

class Job : public TimeWheel::Timer {
 public:
  Job(int s, bool n = false) : Timer(s,n){}
  void callback(){}
};

BOOST_AUTO_TEST_SUITE(TimerTestCases)

BOOST_AUTO_TEST_CASE(testTimeSpan)
{
  TimeWheel::Timer* timerptr = new Job(100);
  TimeWheel::Timer& timer = *timerptr;
  BOOST_CHECK(timer.getTimeSpan() == 100);
}

BOOST_AUTO_TEST_CASE(testInitialState) {
  Job timer(100);
  BOOST_CHECK(timer.isRegistered() == false);
  BOOST_CHECK(timer.needRepeat() == false);
  Job timer2(100, true);
  BOOST_CHECK(timer2.needRepeat() == true);
}

BOOST_AUTO_TEST_SUITE_END()
