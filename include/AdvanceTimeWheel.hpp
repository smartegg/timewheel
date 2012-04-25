#ifndef _NDSL_ADVANCE_TIME_WHEEL_HPP_
#define _NDSL_ADVANCE_TIME_WHEEL_HPP_

#include "TimeWheel.hpp"

namespace ndsl {

class AdvanceTimeWheel {
  public:
    class Timer : public TimeWheel::Timer {
        explicit Timer(int timespan, bool needRepeat = false)
          : TimeWheel::Timer(timespan, needRepeat) {
          for (int i = 0; i < left; i++) {
            left[i] = 0;
          }
        }
        virtual ~Timer() {}

        virtual AdvanceTimeWheel* getTimeWheel() {
          return wh_;
        }
      protected:
        virtual void setTimeWheel(AdvanceTimeWheel* wheel) { wh_ = wheel;}
      private:
        int left[5];
        AdvanceTimeWheel* wh_;
    };

    AdvanceTimeWheel();
    virtual ~AdvanceTimeWheel();

    void run(int milliseconds);

    void addTimer(AdvanceTimeWheel::Timer& timer);

    size_t totalTimers() const;

  private :
    void tick();
    TimeWheel* wheels_;
    TimeWheel::Timer* timers_[4];

    AdvanceTimeWheel(const AdvanceTimeWheel&);
    const AdvanceTimeWheel& operator= (const AdvanceTimeWheel&);
};
} //namespace ndsl
#endif //_NDSL_ADVANCE_TIME_WHEEL_HPP_
