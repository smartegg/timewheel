#ifndef _NDSL_BASIC_TIME_WHEEL_HPP_
#define _NDSL_BASIC_TIME_WHEEL_HPP_

#include <vector>
#include <boost/intrusive/list.hpp>

namespace ndsl {
class BasicTimeWheel {
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

        virtual BasicTimeWheel* getTimeWheel() {
          return wh_;
        }

       protected:
        virtual void setTimeWheel(BasicTimeWheel* wheel) { wh_ = wheel;}
       private:
        int timespan_;
        int rc_;//left rotation time
        bool needRepeat_;
        BasicTimeWheel* wh_;

        friend class BasicTimeWheel;
        Timer(const Timer&);
        Timer& operator=(const Timer&);
        Timer();
    };

  protected:
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
    explicit BasicTimeWheel(int frequence = 100, int wheelSize_ = 1024);
    virtual ~BasicTimeWheel();

    //run  every @frequence ms
    std::vector<Timer*>& tick();
    void addTimer(Timer& timer);

    size_t totalTimers() const;
    size_t wheelSize() const;
    size_t getFrequence() const;
  
  protected:
  
    Spoke* wheel_;
    size_t currentIndex_;
    size_t wheelSize_;
    size_t frequence_;
};

inline size_t BasicTimeWheel::wheelSize() const {
  return wheelSize_;
}

inline size_t BasicTimeWheel::getFrequence() const {
  return frequence_;
}


}//namespace ndsl
#endif // _NDSL_BASIC_TIME_WHEEL_HPP_
