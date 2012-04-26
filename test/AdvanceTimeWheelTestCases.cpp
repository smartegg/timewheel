#include "AdvanceTimeWheelTestCases.hpp"

#include <boost/test/test_tools.hpp>
#include "AdvanceTimeWheel.hpp"

using namespace ndsl;
using namespace std;
typedef AdvanceTimeWheel::Timer Timer;

namespace {

class Job : public Timer {
 public :
  Job(int s, bool f=false) : Timer(s, f){}
  void callback() { 
  }
};

class StopTimer : public Timer {
 public :
  StopTimer(int s, bool f=false) : Timer(s, f){}
  void callback() { 
    using namespace std;
    
    //creat  100ms, 500ms,600ms one-shot timers
    Timer*  timer1 = new Job(100);
    Timer*  timer2 = new Job(500);
    Timer*  timer3 = new Job(600);
    this->getAdvanceTimeWheel()->addTimer(*timer1);
    this->getAdvanceTimeWheel()->addTimer(*timer2);
    this->getAdvanceTimeWheel()->addTimer(*timer3);

    //stop this repeated timer
    this->reset();
  }
};

}//no-name namespace  


void AdvanceTimeWheelTestCases::testAddandDelete() {
  AdvanceTimeWheel  wheel;
  Timer* job(new Job(100));

  wheel.addTimer(*job);
  BOOST_CHECK_EQUAL(wheel.totalTimers() , 1);
  job->stop();
  BOOST_CHECK_EQUAL(wheel.totalTimers() , 0);
}


void AdvanceTimeWheelTestCases::testAddandDelete2() {
  AdvanceTimeWheel wheel;
  vector<AdvanceTimeWheel::Timer* > jobs;

  for(size_t i = 1; i<= 10000; i++) {
    jobs.push_back(new Job(100)); 
    wheel.addTimer(*jobs[i-1]);
    BOOST_CHECK_EQUAL(wheel.totalTimers(), i);
  }
  for(int i = 9999; i>= 0; i--) {
    jobs[i]->stop();
    BOOST_CHECK_EQUAL(wheel.totalTimers() , (size_t)i);
  }
}

void AdvanceTimeWheelTestCases::testTicks() {
  AdvanceTimeWheel* wheel = new AdvanceTimeWheel();
  //creat a one-shot timer
  Timer* timer1(new Job(100,false));

  //creat a one-shot timer
  Timer* timer2(new Job(600,false));

  //creat a one-shot timer
  Timer* timer3(new Job(800,false));

  //creat a one-shot timer
  Timer* timer4(new Job(500,false));

  BOOST_CHECK_EQUAL(wheel->totalTimers() , 0);
  wheel->addTimer(*timer1);
  wheel->addTimer(*timer2);
  wheel->addTimer(*timer3);
  wheel->addTimer(*timer4);
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 4);

/*  tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect);
  driver->mountTimeWheel(wheel);
  driver->start(); */

  //simulate just as if there is a  time_driver drive this wheel.
  for(int i = 0; i < 99; i++) {
    wheel->run(1);
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 4);
  }
  wheel->run(1);//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  for(int i = 0; i < 100; i++) {
  wheel->run(1);
    BOOST_CHECK_MESSAGE(wheel->totalTimers()==3, " " << wheel->totalTimers() << "\t time:" << ( 200+i+1)); 
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);
    
  }

  wheel->run(100);//400
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3);

  wheel->run(100);//500
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);

  for(int i = 0; i < 99; i++) {
    wheel->run(1);//510
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 2);
  }


  wheel->run(1);//600
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

void AdvanceTimeWheelTestCases::testAddRemoveInBaseJob() {
  AdvanceTimeWheel* wheel = new AdvanceTimeWheel();
  
  Timer* job(new StopTimer(100, true));          
  //creat a one-shot timer

  wheel->addTimer(*job);

  //simulate
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); 
  wheel->run(100);//100
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 3); //expired one , create three
  
  wheel->run(100);//200
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); //expired one


  for(int i = 201; i < 600; i++) { 
    wheel->run(1);
    BOOST_CHECK_MESSAGE(wheel->totalTimers()==2, " " << wheel->totalTimers() << "\t time:" << (i)); 
    BOOST_CHECK_EQUAL(wheel->totalTimers(), 2); 
  }
  wheel->run(1);//600
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 1); //expired 
  wheel->run(100);//700
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //expired 
  wheel->run(100);//800
  BOOST_CHECK_EQUAL(wheel->totalTimers(), 0); //no timers
}
//FIXME   add a testcase to check if repeat-timer is legal.


