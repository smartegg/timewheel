#include "TimeHelper.hpp"


#include <string>
#include <sstream>




namespace NDSL {

void handleError(const char* filename,
                        const char* funcname,
                        const int line,
                        bool issyserr) {
  std::stringstream stream; 
  std::string result;
  stream << line -1;//function failed is the line before it.
  stream >> result;


  std::string info("ERROR: ");
  info.append(filename);
  info.append(":");
  info.append(result);
  info.append(std::string("\tin function ") + funcname);
  info.append("() ");
  if (issyserr) {
    perror(info.c_str());
  }
  else {
    std::cerr << info << std::endl;
  }
  exit(-1);  
}

void start_count_time(void) {
#ifdef DEBUG
    printf_elapsed_time();
#endif
}

void printf_elapsed_time(void)
{
#ifdef DEBUG
 static struct timespec start;
 struct timespec curr;
 static int first_call = 1;
 int secs, nsecs;

 if (first_call) {
     first_call = 0;
     if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
         handle_error(true);
   }

   if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
       handle_error(true);

   secs = curr.tv_sec - start.tv_sec;
   nsecs = curr.tv_nsec - start.tv_nsec;
   if (nsecs < 0) {
       secs--;
       nsecs += 1000000000;
   }
   printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
   puts("");
#endif
}

}
