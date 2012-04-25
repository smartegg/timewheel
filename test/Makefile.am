INCLUDES = -I$(top_srcdir)/include
noinst_PROGRAMS = ../bin/test
___bin_test_SOURCES = test.cc  TimerTestCases.cpp TimeWheelTestCases.cpp  AdvanceTimeWheelTestCases.cpp
include_HEADERS = suites.h TimerTestCases.hpp TimeWheelTestCases.hpp  AdvanceTimeWheelTestCases.hpp
EXTRA_DIST = 
# NOTE: link against to local generated lib, MUST use LDADD
___bin_test_LDADD = $(top_builddir)/lib/libtimewheel.a
___bin_test_LDFLAGS = -lboost_unit_test_framework
