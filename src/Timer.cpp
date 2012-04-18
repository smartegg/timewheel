#include "Timer.hpp"
#include "BaseJob.hpp"

namespace NDSL {

int  Timer::expiryAction(void* ptr) {
    //just do nothing
    jobptr_->run(this);
    return 0;
}
}


