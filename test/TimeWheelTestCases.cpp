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
    return 0;
  }
};

class StopTimer : public BaseJob {
 public :
  int run(Timer* timer) {
    using namespace std;
    tr1::shared_ptr<BaseJob> job(new Job());          
    //creat  one-shot 100ms, 500ms,600ms timers
    tr1::shared_ptr<Timer> timer1(new Timer(job, 100,false));
    tr1::shared_ptr<Timer> timer2(new Timer(job, 500,false));
    tr1::shared_ptr<Timer> timer3(new Timer(job, 600,false));
    timer->getTimeWheel()->addTimer(timer1);
    timer->getTimeWheel()->addTimer(timer2);
    timer->getTimeWheel()->addTimer(timer3);
    timer->getTimeWheel()->stopTimer(timer);
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

  //creat a one-shot timer
  tr1::shared_ptr<Timer> timer4(new Timer(job, 500,false));

  BOOST_CHECK(wheel->totalTimers() == 0);
  wheel->addTimer(timer1);
  wheel->addTimer(timer2);
  wheel->addTimer(timer3);
  wheel->addTimer(timer4);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 4);

/*  tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect);
  driver->mountTimeWheel(wheel);
  driver->start();   */

  //simulate just as if there is a time driver drive this wheel.
  wheel->perTickBookKeeping();//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->perTickBookKeeping();//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->perTickBookKeeping();//300
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->perTickBookKeeping();//400
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->perTickBookKeeping();//500
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);


  wheel->perTickBookKeeping();//600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  wheel->perTickBookKeeping();//700
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  wheel->perTickBookKeeping();//800
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);

  wheel->perTickBookKeeping();//900
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);

  wheel->perTickBookKeeping();//1000
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); 
}

void TimeWheelTestCases::testAddRemoveInBaseJob() {
  tr1::shared_ptr<TimeWheel> wheel(new  TimeWheel(100,5));
  tr1::shared_ptr<BaseJob> job(new StopTimer());          
  //creat a one-shot timer
  tr1::shared_ptr<Timer> timer1(new Timer(job, 100,true));

  wheel->addTimer(timer1);

  //simulate
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  wheel->perTickBookKeeping();//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3); //expired one , create two
  
  wheel->perTickBookKeeping();//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); //expired one
  wheel->perTickBookKeeping();//300
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->perTickBookKeeping();//400
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->perTickBookKeeping();//500
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->perTickBookKeeping();//600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); //expired 
  wheel->perTickBookKeeping();//700
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //expired 
  wheel->perTickBookKeeping();//800
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //no timers
}
