INCLUDES = -I$(top_srcdir)/include
noinst_PROGRAMS = ../samples/singeleton ../samples/select ../samples/timefd
___samples_singeleton_SOURCES =  singeleton_timedriver_test.cpp
___samples_select_SOURCES =   TimeDriverBySelect_unit_test.cpp
___samples_timefd_SOURCES =   TimeDriverByTimeFd_unit_test.cpp
#___bin_test_CXXFLAGS = 
EXTRA_DIST = 
# NOTE: link against to local generated lib, MUST use LDADD
___samples_singeleton_LDADD = $(top_builddir)/lib/libtimewheel.a
___samples_select_LDADD = $(top_builddir)/lib/libtimewheel.a
___samples_timefd_LDADD = $(top_builddir)/lib/libtimewheel.a

