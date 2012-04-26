#include "TimeWheelTestCases.hpp"

#include <boost/test/test_tools.hpp>
#include <tr1/memory>
#include <vector>
#include "TimeWheel.hpp"

using namespace ndsl;
using namespace std;
typedef TimeWheel::Timer Timer;

namespace {

class Job : public TimeWheel::Timer {
 public :
  Job(int s, bool f=false) : Timer(s, f){}
  void callback() { 
  }
};

class StopTimer : public TimeWheel::Timer {
 public :
  StopTimer(int s, bool f=false) : Timer(s, f){}
  void callback() { 
    using namespace std;
    
    //creat  100ms, 500ms,600ms one-shot timers
    Timer*  timer1 = new Job(100);
    Timer*  timer2 = new Job(500);
    Timer*  timer3 = new Job(600);
    this->getTimeWheel()->addTimer(*timer1);
    this->getTimeWheel()->addTimer(*timer2);
    this->getTimeWheel()->addTimer(*timer3);

    //stop this repeated timer
    this->stop();
  }
};

class RepeatTimer : public TimeWheel::Timer {
 public: 
  RepeatTimer(int s, bool f = false) : Timer(s, f) {}
  void callback() {
    Timer*  timer1 = new Job(100);
    Timer*  timer3 = new Job(600);
    Timer*  timer2 = new Job(4000);
    this->getTimeWheel()->addTimer(*timer1);
    this->getTimeWheel()->addTimer(*timer2);
    this->getTimeWheel()->addTimer(*timer3);
  }
};

}


void TimeWheelTestCases::testDefaultCtor() {
  TimeWheel wheel;    
  BOOST_CHECK(wheel.getFrequence() == 100);
}

void TimeWheelTestCases::testCtors() {
  TimeWheel wheel(200);
  BOOST_CHECK(wheel.getFrequence() == 200);
}

void TimeWheelTestCases::testAddandDelete() {
  TimeWheel  wheel;
  TimeWheel::Timer* job(new Job(100));
  wheel.addTimer(*job);
  BOOST_CHECK(wheel.totalTimers() == 1);
  job->stop();
  BOOST_CHECK(wheel.totalTimers() == 0);
}


void TimeWheelTestCases::testAddandDelete2() {
  TimeWheel wheel;
  vector<TimeWheel::Timer* > jobs;

  for(size_t i = 1; i<= 10000; i++) {
    jobs.push_back(new Job(100)); 
    wheel.addTimer(*jobs[i-1]);
    BOOST_CHECK(wheel.totalTimers() == i);
  }
  for(int i = 9999; i>= 0; i--) {
    jobs[i]->stop();
    BOOST_CHECK(wheel.totalTimers() == (size_t)i);
  }
}

void TimeWheelTestCases::testTicks() {
  TimeWheel* wheel = new TimeWheel(100,5);
  //creat a one-shot timer
  Timer* timer1(new Job(100,false));

  //creat a one-shot timer
  Timer* timer2(new Job(600,false));

  //creat a one-shot timer
  Timer* timer3(new Job(800,false));

  //creat a one-shot timer
  Timer* timer4(new Job(500,false));

  BOOST_CHECK(wheel->totalTimers() == 0);
  wheel->addTimer(*timer1);
  wheel->addTimer(*timer2);
  wheel->addTimer(*timer3);
  wheel->addTimer(*timer4);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 4);

/*  tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect);
  driver->mountTimeWheel(wheel);
  driver->start();   */

  //simulate just as if there is a  time_driver drive this wheel.
  wheel->run(100);//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//300
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//400
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//500
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);


  wheel->run(100);//600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  wheel->run(100);//700
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1);


  wheel->run(100);//800
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);

  wheel->run(100);//900
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0);

  wheel->run(100);//1000
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); 
}

void TimeWheelTestCases::testAddRemoveInBaseJob() {
  TimeWheel* wheel = new TimeWheel(100,5);
  
  Timer* job(new StopTimer(100, true));          
  //creat a one-shot timer

  wheel->addTimer(*job);

  //simulate
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  wheel->run(100);//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3); //expired one , create two
  
  wheel->run(100);//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); //expired one
  wheel->run(100);//300
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->run(100);//400
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->run(100);//500
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  wheel->run(100);//600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); //expired 
  wheel->run(100);//700
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //expired 
  wheel->run(100);//800
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //no timers
}


void TimeWheelTestCases::testRepeatTimer() {
  TimeWheel* wheel = new TimeWheel(1, 10000);
  Timer* timer(new RepeatTimer(5000, true));
  wheel->addTimer(*timer);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  
  for(int t = 1; t<=4999; t++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  }
  wheel->run(1);//5000
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 4); 
  for(int t = 5001; t< 5100; t++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 4); 
  }
  wheel->run(1);//5100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3); 
  for(int t = 5101; t < 5600; t++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 3); 
  }
  wheel->run(1);//5600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  for(int t = 5601; t < 9000; t++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);
  }
  wheel->run(1);//9000
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  for(int t = 9001; t < 10000; t++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  }
  wheel->run(1);//10000
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 4); 
  delete wheel;
}


