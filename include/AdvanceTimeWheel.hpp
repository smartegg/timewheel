/**
 * @file AdvanceTimeWheel.hpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 15:00:30
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
#ifndef _NDSL_ADVANCE_TIME_WHEEL_HPP_
#define _NDSL_ADVANCE_TIME_WHEEL_HPP_

#include <boost/intrusive/list.hpp>
#include "TimerInterface.hpp"
#include "TimeWheelInterface.hpp"

namespace ndsl {

class InnerTimeWheel;

class AdvanceTimeWheel : public TimeWheelInterface{
  public:
    class Timer : public TimerInterface{
      public:
        explicit Timer(int timespan, bool needRepeat = false);
        virtual ~Timer();
        boost::intrusive::list_member_hook
        <
        boost::intrusive::link_mode<boost::intrusive::auto_unlink>
        > timeHook_;
        virtual void callback() = 0;
        /* --------------------------------------------------------------------------*/
        /**
         * @brief setTimeSpan()
         * set the new timeout time
         * MUST first stop(), then call this function. other action undefined.
         * @Param timespan
         */
        /* ----------------------------------------------------------------------------*/
        int getTimeSpan() const;
        void setTimeSpan(int timespan);
        bool isRegistered() const;
        bool needRepeat() const;
        //just kill the timer,and make the timer in an invalid state
        //so you can not reinsert it into the timewheel
        //reinsert action is undefined.
        virtual void stop();
        AdvanceTimeWheel* getAdvanceTimeWheel() const;
      protected:
        virtual void clear() ;
        //kill the timer, and reset the timer origin time arm, so you can
        //reinert into the TimeWheel
        virtual void reset();
      private:

        InnerTimeWheel* getTimeWheel()const;
        void setAdvanceTimeWheel(AdvanceTimeWheel* wheel);
        void setTimeWheel(InnerTimeWheel* wheel);
        void setTimeDiff(int diff);
        int getRealTimeSpan() const;

        int idx_[5];
        int idxlen_;
        int left_[5];

        int timespan_;
        int rc_;
        bool needRepeat_;
        AdvanceTimeWheel* wh_;
        InnerTimeWheel* inner_;
        int timediff_;

        friend class AdvanceTimeWheel;
        friend class InnerTimeWheel;
        Timer(const Timer&);
        Timer& operator=(const Timer&);
    };
  public:
    AdvanceTimeWheel();
    virtual ~AdvanceTimeWheel();

    virtual int getGranularity() const;

    void run(int milliseconds);

    void addTimer(AdvanceTimeWheel::Timer& timer);
    size_t totalTimers() const;
  private:
    void tick();
    void move_timers(int wheelId);
    InnerTimeWheel* wheels_[5];
    Timer* timers_[4];
    friend class Timer0;
    friend class Timer1;
    friend class Timer2;
    friend class Timer3;
};

inline int AdvanceTimeWheel::Timer::getTimeSpan()const {
  return timespan_;
}

inline void AdvanceTimeWheel::Timer::setTimeSpan(int timespan) {
  timespan_ = timespan;
}

inline bool AdvanceTimeWheel::Timer::isRegistered() const {
  return timeHook_.is_linked() && wh_!= 0 && inner_ != 0;
}

inline bool AdvanceTimeWheel::Timer::needRepeat() const {
  return needRepeat_;
}

inline InnerTimeWheel* AdvanceTimeWheel::Timer::getTimeWheel() const {
  return inner_;
}

inline void AdvanceTimeWheel::Timer::setTimeDiff(int timediff) {
  timediff_ =  timediff;
}


inline int AdvanceTimeWheel::Timer::getRealTimeSpan() const {
  return timediff_ + timespan_;
}

inline AdvanceTimeWheel* AdvanceTimeWheel::Timer::getAdvanceTimeWheel() const {
  return wh_;
}

inline void AdvanceTimeWheel::Timer::setAdvanceTimeWheel(
  AdvanceTimeWheel* wh) {
  wh_  = wh;
}

inline void AdvanceTimeWheel::Timer::setTimeWheel(
  InnerTimeWheel* inner) {
  inner_ = inner;
}

inline int AdvanceTimeWheel::getGranularity() const{
  return 1;
}




} //namespace ndsl
#endif //_NDSL_ADVANCE_TIME_WHEEL_HPP_
