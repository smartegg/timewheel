/**
 * @file TimeDriverBySelect.cpp
 * @brief
 * @author biyu<lazysmartegg@gmail.com>
 * @version 1.0
 * @date Wed, 02 May 2012 17:03:46
 * @copyright Copyright (C) 2012 smartegg<lazysmartegg@gmail.com>
 */
#include "TimeDriverBySelect.hpp"

#include "TimeHelper.hpp"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>




namespace ndsl {

TimeDriverBySelect::TimeDriverBySelect(long granularity)
  : TimeDriver(granularity) {

}

TimeDriverBySelect::~TimeDriverBySelect() {

}

int TimeDriverBySelect::start() {
  struct timeval timeout;


  timeout.tv_sec = granularity_ / 1000;
  timeout.tv_usec = (granularity_ % 1000) * 1000;//milli to micro

  printf_elapsed_time();

  for (;;) {
    int r = ::select(0, NULL, NULL, NULL, &timeout);

    if (r < 0) {
      if (r == -1 && errno == EINTR) {
        continue;
      }

      handle_error(true);
    }

    printf_elapsed_time();
    tick();

    //restart
    timeout.tv_sec = granularity_ / 1000;
    timeout.tv_usec = (granularity_ % 1000) * 1000;//milli to micro
  }

  return 0;
}

}
