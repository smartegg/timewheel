#ifndef _NDSL_TIME_WHEEL_INTERFACE_HPP_
#define _NDSL_TIME_WHEEL_INTERFACE_HPP_
namespace ndsl {

class TimeWheelInterface {
  public:
    TimeWheelInterface() {}
    virtual ~TimeWheelInterface() {}

    virtual void run(int milliseconds) = 0;
    virtual int  getGranularity()const = 0;

};
}//namespace ndsl
#endif //_NDSL_TIME_WHEEL_INTERFACE_HPP_
