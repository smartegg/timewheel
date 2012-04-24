#ifndef _NDSL_TIME_WHEEL_H_
#define _NDSL_TIME_WHEEL_H_

#include <boost/intrusive/list.hpp>

namespace ndsl {

class TimeWheel {
  public:
    class Timer {
      public:
        Timer(int timespan) : timespan_(timespan) {}
        boost::intrusive::list_member_hook<<link_mode<auto_unlink> > timeHook_;

        //MUST NOT free the timer's memory when callback()!
        virtual void callback() = 0;
        int getTimeSpan()const {
          return timespan_;
        }

        void stop() {
          if (timeHook_.is_linked())
            timeHook_.unlink();
        }

      private:
        int timespan_;
        int rt_;//left rotation time
    };
  private:
    typedef boost::intrusive::list<Timer, boost::intrusive::list_member_hook<<link_mode<auto_unlink> >,&Timer::timeHook_> Spoke;
  public:
    TimeWheel(int frequence = 100, int wheelSize_ = 1024);
    virtual ~TimeWheel();

    void run(int milliseconds);

    void addTimer(Timer& timer);
//    void deleteTimer(Timer& timer);

    size_t totalTimers() const;
    size_t wheelSize() const;
    size_t getFrequence() const;
  
  private:
    int tick();

    Spoke* wheel_;
    int currentIndex_;
    size_t wheelSize_;
    size_t frequence_;
    size_t timers_;
    
};

inline TimeWheel::totalTimers() {
  return timers_;
}

inline TimeWheel::wheelSize() {
  return wheelSize_;
}

inline TimeWheel::getFrequence() {
  return frequence_;
}

}
#endif
