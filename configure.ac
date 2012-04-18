#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([timewheel], [1.0], [lazysmartegg@gmail.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(timewheel, 1.0)
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_RANLIB
AC_PROG_MKDIR_P

AC_ARG_ENABLE(debug, [  --enable-debug	Enable DEBUG output. ],
	[ CXXFLAGS="-g2 -O0 -DDEBUG -Wall -Werror -std=c++0x" ],
	[ CXXFLAGS="-O3 -Wall -Werror -std=c++0x" ])
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lboost_unit_test_framework':
AC_CHECK_LIB([boost_unit_test_framework], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SSIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime memset select])

AC_CONFIG_FILES([Makefile
                src/Makefile
		samples/Makefile
                 test/Makefile])
AC_OUTPUT
