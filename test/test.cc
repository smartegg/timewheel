/**
 * @file test.cc
 * @comment
 * unit test suites for timewheel project.
 *
 */

#include <boost/test/included/unit_test.hpp>
#include <boost/test/debug.hpp>

using boost::unit_test::test_suite;
using boost::debug::detect_memory_leaks;

#include "suites.h"

test_suite *init_unit_test_suite(int argc, char *argv[])
{
	// test suites for class Agent
	test_suite *suites = BOOST_TEST_SUITE("Unit test for project timewheel");
	detect_memory_leaks(true);
	
	// organize all test suites
	suites->add(new MyTestSuit());
	return suites;
}
