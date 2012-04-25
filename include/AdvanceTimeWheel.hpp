#ifndef _NDSL_ADVANCE_TIME_WHEEL_HPP_
#define _NDSL_ADVANCE_TIME_WHEEL_HPP_

#include "BasicTimeWheel.hpp"

namespace ndsl {

class InnerTimeWheel;
class Timer0;
class Timer1;
class Timer2;
class Timer3;


class AdvanceTimeWheel {
  public:
    class Timer : public BasicTimeWheel::Timer {
     public:
        explicit Timer(int timespan = 100, bool needRepeat = false)
          : BasicTimeWheel::Timer(timespan, needRepeat) {
          for (int i = 0; i < 5; i++) {
            left[i] = 0;
          }
        }
        virtual ~Timer() {}

        virtual AdvanceTimeWheel* getAdvanceTimeWheel() {
          return wh_;
        }
      protected:
        virtual void setAdvanceTimeWheel(AdvanceTimeWheel* wheel) { wh_ = wheel;}
      private:
        int left[5];
        int preId;
        AdvanceTimeWheel* wh_;

        friend class AdvanceTimeWheel;
        //useless these setter/getter from parents.
//        void setTimeWheel(BasicTimeWheel*);
//        BasicTimeWheel* getTimeWheel();
    };

    AdvanceTimeWheel();
    virtual ~AdvanceTimeWheel();

    void run(int milliseconds);

    void addTimer(AdvanceTimeWheel::Timer& timer);

    size_t totalTimers() const;

  private :
    void tick();
    void move_timers(int wheelId) ;
    InnerTimeWheel* wheels_[5];
    AdvanceTimeWheel::Timer* timers_[4];
    

    friend class Timer0;
    friend class Timer1;
    friend class Timer2;
    friend class Timer3;

    AdvanceTimeWheel(const AdvanceTimeWheel&);
    const AdvanceTimeWheel& operator= (const AdvanceTimeWheel&);
};
} //namespace ndsl
#endif //_NDSL_ADVANCE_TIME_WHEEL_HPP_
