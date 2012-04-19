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
#include "BaseJobTestCases.hpp"
#include "TimeWheelTestCases.hpp"
#include "SingeletonTimeDriverTestCases.hpp"
#include "TimeDriverByTimeFdTestCases.hpp"
#include "TimeDriverBySelectTestCases.hpp"

class MyTestSuit : public test_suite
{
public:
	MyTestSuit() : test_suite("MyTestSuit")
	{
		boost::shared_ptr<TimerTestCases> instance(new TimerTestCases());
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testJobValue1, instance));
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testJobValue2, instance));
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testIntervalValue, instance));
		add(BOOST_CLASS_TEST_CASE(&TimerTestCases::testRestartValue, instance));

		boost::shared_ptr<BaseJobTestCases> instance_basejob(new BaseJobTestCases());
    add(BOOST_CLASS_TEST_CASE(&BaseJobTestCases::testCreate, instance_basejob)); 

    boost::shared_ptr<TimeWheelTestCases> instance_timewheel(new TimeWheelTestCases());
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testDefaultCtor, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testCtors, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddandDelete, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddandDelete2, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testIllegalDelete, instance_timewheel)); 
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testTicks, instance_timewheel));
    add(BOOST_CLASS_TEST_CASE(&TimeWheelTestCases::testAddRemoveInBaseJob, instance_timewheel));


    boost::shared_ptr<SingeletonTimeDriverTestCases> instance_singeleton(new SingeletonTimeDriverTestCases());
    add(BOOST_CLASS_TEST_CASE(&SingeletonTimeDriverTestCases::testGranualarity, instance_singeleton));
    add(BOOST_CLASS_TEST_CASE(&SingeletonTimeDriverTestCases::testMountWheel, instance_singeleton));

    boost::shared_ptr<TimeDriverByTimeFdTestCases> instance_timefd(new TimeDriverByTimeFdTestCases());
    add(BOOST_CLASS_TEST_CASE(&TimeDriverByTimeFdTestCases::testGranualarity, instance_timefd));
    add(BOOST_CLASS_TEST_CASE(&TimeDriverByTimeFdTestCases::testMountWheel, instance_timefd));

    boost::shared_ptr<TimeDriverBySelectTestCases> instance_select(new TimeDriverBySelectTestCases());
    add(BOOST_CLASS_TEST_CASE(&TimeDriverBySelectTestCases::testGranualarity, instance_select));
    add(BOOST_CLASS_TEST_CASE(&TimeDriverBySelectTestCases::testMountWheel, instance_select)); 

	}
};

#endif
