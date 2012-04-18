#ifndef NDSL_BASEJOB_H
#define NDSL_BASEJOB_H

namespace NDSL {

class Timer;
class BaseJob {
 public:
  virtual ~BaseJob() {}
  virtual int run(Timer*) {return 0;}
};

}
#endif
