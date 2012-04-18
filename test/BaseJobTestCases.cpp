#include "BaseJobTestCases.hpp"

#include <boost/test/test_tools.hpp>
#include "TimerUtil.hpp"
using namespace NDSL;

void BaseJobTestCases::testCreate() {
  BaseJob job;
  BOOST_CHECK(job.run(0) == 0);
}

