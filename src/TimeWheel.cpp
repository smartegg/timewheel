#include "TimeWheel.hpp"

#include <cassert>
#include <vector>



namespace ndsl {

using namespace boost;

TimeWheel::TimeWheel(int frequence, int wheelSize)
  :  BasicTimeWheel(frequence,wheelSize) {
}

TimeWheel::~TimeWheel() {
}


/**
 * @brief
 * every @frequent  ms, this will be invoked
 */

int TimeWheel::tick() {
  std::vector<Timer*>& waits = BasicTimeWheel::tick();

  for (size_t i = 0; i < waits.size(); i++) {
    waits[i]->callback();
  }

  for (size_t i = 0; i< waits.size(); i++) {
    waits[i]->stop();
  }

  return 0;
}


void TimeWheel::run(int milliseconds) {
  int nc = (milliseconds + frequence_ -1 )/ frequence_;
  if (nc  == 0) nc++;
  for(int i = 0; i < nc; i++)
    tick();
}


}
