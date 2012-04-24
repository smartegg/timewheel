#ifndef _NDSL_TIME_WHEEL_H_
#define _NDSL_TIME_WHEEL_H_

#include <boost/intrusive/list.hpp>

namespace ndsl {
class TimeWheel {
  public:
    class Timer {
      public:
        explicit Timer(int timespan, bool needRepeat = false) 
            : timespan_(timespan),
              needRepeat_(needRepeat_) ,
              wh_(0) {
        }

        virtual ~Timer() {}

        boost::intrusive::list_member_hook
        <
            boost::intrusive::link_mode<boost::intrusive::auto_unlink> 
        > timeHook_;

        //MUST NOT free the timer's memory when callback()!
        virtual void callback()  = 0;

        int getTimeSpan()const {
          return timespan_;
        }

        bool isRegistered() const {
          return timeHook_.is_linked();
        }

        bool needRepeat() const {
          return needRepeat_;
        }

        void stop() {
          if (timeHook_.is_linked())
            timeHook_.unlink();
          wh_ = 0;
        }

        TimeWheel* getTimeWheel() {
          return wh_;
        }

      private:
        void setTimeWheel(TimeWheel* wheel) { wh_ = wheel;}
        int timespan_;
        int rc_;//left rotation time
        bool needRepeat_;
        TimeWheel* wh_;

        friend class TimeWheel;
        Timer(const Timer&);
        Timer& operator=(const Timer&);
        Timer();
    };
  private:
    typedef boost::intrusive::list
        <
          Timer, 
          boost::intrusive::member_hook
          <
            Timer,
            boost::intrusive::list_member_hook
            <
                boost::intrusive::link_mode<boost::intrusive::auto_unlink> 
            >,
            &Timer::timeHook_
          >,
          boost::intrusive::constant_time_size<false> 
      > Spoke;
  public:
    explicit TimeWheel(int frequence = 100, int wheelSize_ = 1024);
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
    size_t currentIndex_;
    size_t wheelSize_;
    size_t frequence_;
};

inline size_t TimeWheel::wheelSize() const {
  return wheelSize_;
}

inline size_t TimeWheel::getFrequence() const {
  return frequence_;
}


}
#endif
