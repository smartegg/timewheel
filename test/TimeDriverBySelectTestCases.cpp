#include "TimeDriverBySelectTestCases.hpp"


#include "TimerUtil.hpp"
#include <boost/test/test_tools.hpp>
#include <tr1/memory>
using namespace NDSL;

void TimeDriverBySelectTestCases::testGranualarity() {
  std::tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect());
  BOOST_CHECK(driver->getGranularity() == TimeDriver::DEFAULT_GRANULARITY); 
}


void TimeDriverBySelectTestCases::testMountWheel() {
  std::tr1::shared_ptr<TimeDriver> driver(new TimeDriverBySelect());

  for (int i = 0; i < 100; i++) {
    std::tr1::shared_ptr<TimeWheel> ptr(new TimeWheel());
    driver->mountTimeWheel(ptr);
  }

  BOOST_CHECK(driver->totalWheels() == 100);

}
