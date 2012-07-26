INCLUDES = -I$(top_srcdir)/include
noinst_PROGRAMS =  ../samples/select  ../samples/asiotimer ../samples/timefd
___samples_select_SOURCES =   TimeDriverBySelect_unit_test.cpp
___samples_asiotimer_SOURCES = TimeDriverByAsioTimer.cpp
___samples_timefd_SOURCES = TimeDriverByTimeFd.cpp
#___bin_test_CXXFLAGS = 
___samples_asiotimer_LDFLAGS = -pthread
EXTRA_DIST = 
# NOTE: link against to local generated lib, MUST use LDADD
___samples_select_LDADD = $(top_builddir)/lib/libtimewheel.a
___samples_asiotimer_LDADD = $(top_builddir)/lib/libtimewheel.a /usr/local/lib/libboost_system.a
___samples_timefd_LDADD = $(top_builddir)/lib/libtimewheel.a
