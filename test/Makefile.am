INCLUDES = -I$(top_srcdir)/include
noinst_PROGRAMS = ../bin/test
___bin_test_SOURCES = test.cc  TimerTestCases.cpp TimeWheelTestCases.cpp BaseJobTestCases.cpp SingeletonTimeDriverTestCases.cpp TimeDriverByTimeFdTestCases.cpp TimeDriverBySelectTestCases.cpp
include_HEADERS = suites.h TimerTestCases.hpp TimeWheelTestCases.hpp BaseJobTestCases.hpp SingeletonTimeDriverTestCases.hpp TimeDriverByTimeFdTestCases.hpp TimeDriverBySelectTestCases.hpp
#___bin_test_CXXFLAGS = 
EXTRA_DIST = 
# NOTE: link against to local generated lib, MUST use LDADD
___bin_test_LDADD = $(top_builddir)/lib/libtimewheel.a
___bin_test_LDFLAGS = -lboost_unit_test_framework
