/**
 * @file suites.h
 * @comment
 *	All tests
 * 
 * @author niexw
 */
#ifndef _XCOM_TEST_SUITES_H_
#define _XCOM_TEST_SUITES_H_

#include "TimerTestCases.hpp"
#include "TimeWheelTestCases.hpp"

class MyTestSuit : public test_suite
{
public:
	MyTestSuit() : test_suite("MyTestSuit")
	{
		boost::shared_ptr<TimerTestCases> instance(new TimerTestCases());
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testTimeSpan, instance));
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testInitialState, instance));


    boost::shared_ptr<TimeWheelTestCases> instance_timewheel(new TimeWheelTestCases());
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testDefaultCtor, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testCtors, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddandDelete, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddandDelete2, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testTicks, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddRemoveInBaseJob, instance_timewheel));


	}
};

#endif
