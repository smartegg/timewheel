#include "TimeWheelTestCases.hpp"

#include <boost/test/test_tools.hpp>
#include <tr1/memory>
#include <vector>
#include "TimerUtil.hpp"

using namespace NDSL;
using namespace std;

namespace {

class Job : public BaseJob {
 public :
  int run(Timer* ) {
    puts("heelo");
    return 0;
  }
};
}


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
  wheel.addTimer(timer);
  BOOST_CHECK(wheel.totalTimers() == 1);
  wheel.stopTimer(timer);
  BOOST_CHECK(wheel.totalTimers() == 0);
}

void TimeWheelTestCases::testIllegalDelete() {
  TimeWheel wheel;
  tr1::shared_ptr<BaseJob> job(new BaseJob());                                                                                         
  tr1::shared_ptr<Timer> timer(new Timer(job, 100,true));
  //insert a timer which is not inserted
  BOOST_CHECK(wheel.stopTimer(timer) == -1);
}

void TimeWheelTestCases::testAddandDelete2() {
  TimeWheel wheel;
  vector<tr1::shared_ptr<Timer> > timers;

  for(int i = 1; i<= 10000; i++) {
    tr1::shared_ptr<BaseJob> job(new BaseJob());          
    tr1::shared_ptr<Timer> timer(new Timer(job, 100,true));
    timers.push_back(timer);
    wheel.addTimer(timer);
    BOOST_CHECK(wheel.totalTimers() == i);
  }
  for(int i = 9999; i>= 0; i--) {
    int status = wheel.stopTimer(timers[i]);
    BOOST_CHECK(status == 0);
    BOOST_CHECK(wheel.totalTimers() == i);
  }
}

void TimeWheelTestCases::testTicks() {
  tr1::shared_ptr<TimeWheel> wheel(new  TimeWheel(100,5));
  tr1::shared_ptr<BaseJob> job(new Job());          
  //creat a one-shot timer
  tr1::shared_ptr<Timer> timer1(new Timer(job, 100,false));

  //creat a one-shot timer
  tr1::shared_ptr<Timer> timer2(new Timer(job, 600,false));

  //creat a one-shot timer
  tr1::shared_ptr<Timer> timer3(new Timer(job, 800,false));

  BOOST_CHECK(wheel->totalTimers() == 0);
  wheel->addTimer(timer1);
  wheel->addTimer(timer2);
  wheel->addTimer(timer3);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

/*  tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect);
  driver->mountTimeWheel(wheel);
  driver->start();   */

  //simulate just as if there is a time driver drive this wheel.
  int ndeleted = 0;
  ndeleted =  wheel->perTickBookKeeping();//100
  BOOST_CHECK_EQUAL(ndeleted, 1);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);

  ndeleted = wheel->perTickBookKeeping();//200
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);

  ndeleted = wheel->perTickBookKeeping();//300
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);

  ndeleted = wheel->perTickBookKeeping();//400
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);

  ndeleted = wheel->perTickBookKeeping();//500
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);


  ndeleted = wheel->perTickBookKeeping();//600
  BOOST_CHECK_EQUAL(ndeleted, 1);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  ndeleted = wheel->perTickBookKeeping();//700
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  ndeleted = wheel->perTickBookKeeping();//800
  BOOST_CHECK_EQUAL(ndeleted, 1);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);

  ndeleted = wheel->perTickBookKeeping();//900
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);


  ndeleted = wheel->perTickBookKeeping();//1000
  BOOST_CHECK_EQUAL(ndeleted, 0);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); 



}
