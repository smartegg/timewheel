INCLUDES = -I$(top_srcdir)/include
noinst_PROGRAMS = ../bin/test
___bin_test_SOURCES =  TimerTestCases.cpp TimeWheelTestCases.cpp  AdvanceTimeWheelTestCases.cpp TestMain.cpp
EXTRA_DIST = 
# NOTE: link against to local generated lib, MUST use LDADD
___bin_test_LDADD = $(top_builddir)/lib/libtimewheel.a
___bin_test_LDFLAGS = -lboost_unit_test_framework
