#include "TimeWheelTestCases.hpp"

#include <boost/test/test_tools.hpp>
#include <tr1/memory>
#include <vector>
#include "TimerUtil.hpp"

using namespace NDSL;
using namespace std;;


void TimeWheelTestCases::testDefaultCtor() {
  TimeWheel wheel;    
  BOOST_CHECK(wheel.getGranularity() == 100);
}

void TimeWheelTestCases::testCtors() {
  TimeWheel wheel(200);
  BOOST_CHECK(wheel.getGranularity() == 200);
}

void TimeWheelTestCases::testAddandDelete() {
  TimeWheel  wheel;
  tr1::shared_ptr<BaseJob> job(new BaseJob());                                                                                         
  tr1::shared_ptr<Timer> timer(new Timer(job, 100,true));
  TimeWheel::TimerId id = wheel.addTimer(timer);
  BOOST_CHECK(wheel.totalTimers() == 1);
  wheel.stopTimer(id);
  BOOST_CHECK(wheel.totalTimers() == 0);
}

void TimeWheelTestCases::testIllegalDelete() {
  TimeWheel wheel;
  TimeWheel::TimerId id;
  id.first = -1;
  BOOST_CHECK(wheel.stopTimer(id) == -1);
}

void TimeWheelTestCases::testAddandDelete2() {
  TimeWheel wheel;
  vector<TimeWheel::TimerId> ids;

  for(int i = 1; i<= 10000; i++) {
    tr1::shared_ptr<BaseJob> job(new BaseJob());          
    tr1::shared_ptr<Timer> timer(new Timer(job, 100,true));
    ids.push_back(wheel.addTimer(timer));
    BOOST_CHECK(wheel.totalTimers() == i);
  }
  for(int i = 9999; i>= 0; i--) {
    int status = wheel.stopTimer(ids[i]);
    BOOST_CHECK(status == 0);
    BOOST_CHECK(wheel.totalTimers() == i);
  }
}
