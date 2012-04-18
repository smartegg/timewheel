#include "SingeletonTimeDriverTestCases.hpp"

#include "TimerUtil.hpp"
#include <boost/test/test_tools.hpp>
#include <tr1/memory>
using namespace NDSL;

void SingeletonTimeDriverTestCases::testGranualarity() {
  TimeDriver* driver = SingeletonTimeDriver::getInstance();
  BOOST_CHECK(driver != 0);
  BOOST_CHECK(driver->getGranularity() == TimeDriver::DEFAULT_GRANULARITY); 
}

void SingeletonTimeDriverTestCases::testMountWheel() {
  TimeDriver* driver = SingeletonTimeDriver::getInstance();
  BOOST_CHECK(driver != 0);

  for (int i = 0; i < 100; i++) {
    std::tr1::shared_ptr<TimeWheel> ptr(new TimeWheel());
    driver->mountTimeWheel(ptr);
  }

  BOOST_CHECK(driver->totalWheels() == 100);
}

