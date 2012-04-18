#include "TimerTestCases.hpp"

#include "TimerUtil.hpp"
#include <boost/test/test_tools.hpp>

using namespace NDSL;
using namespace std::tr1;

void TimerTestCases::testJobValue1() {
  shared_ptr<BaseJob> job(new BaseJob()); 
  Timer timer1(job, 100, true);
  BOOST_CHECK(timer1.getJob().get()  == job.get());
}

void TimerTestCases::testJobValue2() {
  Timer timer2(shared_ptr<BaseJob>((BaseJob*)0), 100, true);
  BOOST_CHECK(timer2.getJob().get()  == 0);
}

void TimerTestCases::testIntervalValue() {
  shared_ptr<BaseJob> job(new BaseJob()); 
  Timer timer(job, 100, true);
  BOOST_CHECK(timer.getInterval() == 100);
}

void TimerTestCases::testRestartValue() {
  shared_ptr<BaseJob> job(new BaseJob()); 
  Timer timer(job, 100, true);
  BOOST_CHECK(timer.needRestart() == true);
  Timer timer2(job, 100, false);
  BOOST_CHECK(timer2.needRestart() == false);
}
