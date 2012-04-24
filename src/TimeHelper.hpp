#ifndef NDSL_TIME_HELPER_H
#define NDSL_TIME_HELPER_H

#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace NDSL {

extern void handleError(const char* filename,const char* funcname,
                        const int line,
                        bool  issyscall);
extern void printf_elapsed_time(void);
extern void start_count_time(void);

}



#define handle_error(issyserr) \
       do {NDSL::handleError(__FILE__, __FUNCTION__, __LINE__, issyserr);  } while (0)
#define dbg(a,b) \
    std::cout <<#a <<'\t' << a << '\t' << #b << '\t' << b << std::endl





#endif
