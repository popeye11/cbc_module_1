/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2019-2020
 *
 * @file        test_pid.cpp
 * @brief       Unit Tests for the PID loop.
 *
 * @author      Fuchs, Daniel <Daniel.Fuchs@toptica.com>
 * @author      Hager, Manfred <Manfred.Hager@toptica.com>
 * @author      Hempel, Felix <Felix.Hempel@toptica.com>
 * @author      Lopes, Emilio <Emilio.Lopes@toptica.com>
 * @author      Rehme, Paul <Paul.Rehme@toptica.com>
 * @author      Roggenbuck, Axel <Axel.Roggenbuck@toptica.com>
 * @author      Zhang, Xiaodong <Xiaodong.Zhang@toptica.com>
 *
 ******************************************************************************/
#include <boost/test/unit_test.hpp>

#include <tsp/iir.hpp>
#include <tsp/pid.hpp>
#include <tsp/plant.hpp>

#include <test_data.hpp>

#include <limits>


using namespace toptica::tsp::pid;
using boost::unit_test::tolerance;
using boost::test_tools::fpc::percent_tolerance;

BOOST_AUTO_TEST_SUITE(PID)

    BOOST_AUTO_TEST_CASE(pid_default) {
        BOOST_TEST_MESSAGE("PID: Instantiate default PID-loop and check values");

        pid<float> pid{1};

        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);
        BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), std::numeric_limits<float>::lowest());
        BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), std::numeric_limits<float>::max());
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
    }

    BOOST_AUTO_TEST_CASE(
            pid_constructor,
            * boost::unit_test::depends_on("PID/pid_default")) {
        BOOST_TEST_MESSAGE("PID: Instantiate PID-loop with different values");

        {
            pid<float> pid{
                11,
                22};

            BOOST_CHECK_EQUAL(pid.get_state(), "idle");
            BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 11);
            BOOST_CHECK_EQUAL(pid.get_gain(), 1);
            BOOST_CHECK_EQUAL(pid.get_p(), 22);
            BOOST_CHECK_EQUAL(pid.get_i(), 0);
            BOOST_CHECK_EQUAL(pid.get_d(), 0);
            BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), std::numeric_limits<float>::lowest());
            BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), std::numeric_limits<float>::max());
            BOOST_CHECK_EQUAL(pid.is_limited(), false);
        }
        {
            pid<float> pid{
                33,
                44,
                55};

            BOOST_CHECK_EQUAL(pid.get_state(), "idle");
            BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 33);
            BOOST_CHECK_EQUAL(pid.get_gain(), 1);
            BOOST_CHECK_EQUAL(pid.get_p(), 44);
            BOOST_CHECK_EQUAL(pid.get_i(), 55);
            BOOST_CHECK_EQUAL(pid.get_d(), 0);
            BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), std::numeric_limits<float>::lowest());
            BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), std::numeric_limits<float>::max());
            BOOST_CHECK_EQUAL(pid.is_limited(), false);
        }
        {
            pid<float> pid{
                66,
                77,
                88,
                99};

            BOOST_CHECK_EQUAL(pid.get_state(), "idle");
            BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 66);
            BOOST_CHECK_EQUAL(pid.get_gain(), 1);
            BOOST_CHECK_EQUAL(pid.get_p(), 77);
            BOOST_CHECK_EQUAL(pid.get_i(), 88);
            BOOST_CHECK_EQUAL(pid.get_d(), 99);
            BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), std::numeric_limits<float>::lowest());
            BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), std::numeric_limits<float>::max());
            BOOST_CHECK_EQUAL(pid.is_limited(), false);
        }
    }

    BOOST_AUTO_TEST_CASE(pid_states) {
        BOOST_TEST_MESSAGE("PID: Check state transitions");

        pid<float> pid{1};

        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        pid.disable();
        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        pid.hold();
        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        pid.reset();
        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        pid.enable();
        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        pid.enable();
        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        pid.reset();
        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        pid.disable();
        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        pid.enable();
        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        pid.hold();
        BOOST_CHECK_EQUAL(pid.get_state(), "hold");
        pid.hold();
        BOOST_CHECK_EQUAL(pid.get_state(), "hold");
        pid.reset();
        BOOST_CHECK_EQUAL(pid.get_state(), "hold");
        pid.enable();
        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        pid.hold();
        BOOST_CHECK_EQUAL(pid.get_state(), "hold");
        pid.disable();
        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
    }

    BOOST_AUTO_TEST_CASE(
            pid_p,
            * boost::unit_test::depends_on("PID/pid_default")) {
        BOOST_TEST_MESSAGE("PID: Check proportional term");

        pid<float> pid{1};

        pid.set_p(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0);
        BOOST_CHECK_EQUAL(pid.run(1000), 0);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), 0);
        BOOST_CHECK_EQUAL(pid.run(-1000), 0);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 1000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        BOOST_CHECK_EQUAL(pid.run(-1000), -1000);

        pid.set_p(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 1000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        BOOST_CHECK_EQUAL(pid.run(-1000), -1000);

        pid.set_p(2);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 2);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 2);
        BOOST_CHECK_EQUAL(pid.run(1000), 2000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.run(-1000), -2000);

        pid.set_p(-1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), -1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), -1);
        BOOST_CHECK_EQUAL(pid.run(1000), -1000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), 1);
        BOOST_CHECK_EQUAL(pid.run(-1000), 1000);
    }

    BOOST_AUTO_TEST_CASE(
            pid_p_gain,
            * boost::unit_test::depends_on("PID/pid_p")) {
        BOOST_TEST_MESSAGE("PID: Check gain for proportional term");

        pid<float> pid{1};

        pid.set_gain(3);
        pid.set_p(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 3);
        BOOST_CHECK_EQUAL(pid.get_p(), 1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 3);
        BOOST_CHECK_EQUAL(pid.run(1000), 3000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), -3);
        BOOST_CHECK_EQUAL(pid.run(-1000), -3000);
    }

    BOOST_AUTO_TEST_CASE(
            pid_p_time,
            * boost::unit_test::depends_on("PID/pid_p")) {
        BOOST_TEST_MESSAGE("PID: Check timing for proportional term");

        pid<float> pid{1000};

        pid.set_p(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1000);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 1);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 1000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        BOOST_CHECK_EQUAL(pid.run(-1000), -1000);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i,
            * boost::unit_test::depends_on("PID/pid_default")) {
        BOOST_TEST_MESSAGE("PID: Check integral term");

        pid<float> pid{1};

        pid.set_i(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0);
        BOOST_CHECK_EQUAL(pid.run(1000), 0);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), 0);
        BOOST_CHECK_EQUAL(pid.run(-1000), 0);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0.5);
        BOOST_CHECK_EQUAL(pid.run(1000), 501);
        BOOST_CHECK_EQUAL(pid.run(0), 1001);
        BOOST_CHECK_EQUAL(pid.run(-1), 1000.5);
        BOOST_CHECK_EQUAL(pid.run(-1000), 500);
        BOOST_CHECK_EQUAL(pid.run(0), 0);

        pid.set_i(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0.5);
        BOOST_CHECK_EQUAL(pid.run(1000), 501);
        BOOST_CHECK_EQUAL(pid.run(0), 1001);
        BOOST_CHECK_EQUAL(pid.run(-1), 1000.5);
        BOOST_CHECK_EQUAL(pid.run(-1000), 500);
        BOOST_CHECK_EQUAL(pid.run(0), 0);

        pid.set_i(2);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 2);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 1002);
        BOOST_CHECK_EQUAL(pid.run(0), 2002);
        BOOST_CHECK_EQUAL(pid.run(-1), 2001);
        BOOST_CHECK_EQUAL(pid.run(-1000), 1000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);

        pid.set_i(-1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), -1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), -0.5);
        BOOST_CHECK_EQUAL(pid.run(1000), -501);
        BOOST_CHECK_EQUAL(pid.run(0), -1001);
        BOOST_CHECK_EQUAL(pid.run(-1), -1000.5);
        BOOST_CHECK_EQUAL(pid.run(-1000), -500);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_switch_coefficient,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check integral term (switch coefficient)");

        pid<float> pid{1};

        pid.set_i(1);
        pid.enable();

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0.5);

        pid.set_i(2);

        BOOST_CHECK_EQUAL(pid.run(0), 1);
        BOOST_CHECK_EQUAL(pid.run(0), 1);
        BOOST_CHECK_EQUAL(pid.run(1), 2);

        pid.set_i(-1);

        BOOST_CHECK_EQUAL(pid.run(0), 3);
        BOOST_CHECK_EQUAL(pid.run(0), 3);
        BOOST_CHECK_EQUAL(pid.run(1), 2.5);
        BOOST_CHECK_EQUAL(pid.run(0), 2);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_gain,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check gain for integral term");

        pid<float> pid{1};

        pid.set_gain(4);
        pid.set_i(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 4);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 2);
        BOOST_CHECK_EQUAL(pid.run(1000), 2004);
        BOOST_CHECK_EQUAL(pid.run(0), 4004);
        BOOST_CHECK_EQUAL(pid.run(-1), 4002);
        BOOST_CHECK_EQUAL(pid.run(-1000), 2000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_time,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check timing for integral term");

        pid<float> pid{1000};

        pid.set_i(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1000);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 500);
        BOOST_CHECK_EQUAL(pid.run(1000), 501000);
        BOOST_CHECK_EQUAL(pid.run(0), 1001000);
        BOOST_CHECK_EQUAL(pid.run(-1), 1000500);
        BOOST_CHECK_EQUAL(pid.run(-1000), 500000);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_reset,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check reset for integral term");

        pid<float> pid{1};

        pid.set_i(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(1), 0.5);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(1000), 500);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(-1), -0.5);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(-1000), -500);
    }

    BOOST_AUTO_TEST_CASE(
            pid_d,
            * boost::unit_test::depends_on("PID/pid_default")) {
        BOOST_TEST_MESSAGE("PID: Check derivative term");

        pid<float> pid{1};

        pid.set_d(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "idle");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 0);
        BOOST_CHECK_EQUAL(pid.run(1000), 0);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(-1), 0);
        BOOST_CHECK_EQUAL(pid.run(-1000), 0);
        BOOST_CHECK_EQUAL(pid.run(0), 0);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 999);
        BOOST_CHECK_EQUAL(pid.run(0), -1000);
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        BOOST_CHECK_EQUAL(pid.run(-1000), -999);
        BOOST_CHECK_EQUAL(pid.run(0), 1000);

        pid.set_d(1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        BOOST_CHECK_EQUAL(pid.run(1000), 999);
        BOOST_CHECK_EQUAL(pid.run(0), -1000);
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        BOOST_CHECK_EQUAL(pid.run(-1000), -999);
        BOOST_CHECK_EQUAL(pid.run(0), 1000);

        pid.set_d(2);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 2);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 2);
        BOOST_CHECK_EQUAL(pid.run(1000), 1998);
        BOOST_CHECK_EQUAL(pid.run(0), -2000);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.run(-1000), -1998);
        BOOST_CHECK_EQUAL(pid.run(0), 2000);

        pid.set_d(-1);

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), -1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), -1);
        BOOST_CHECK_EQUAL(pid.run(1000), -999);
        BOOST_CHECK_EQUAL(pid.run(0), 1000);
        BOOST_CHECK_EQUAL(pid.run(-1), 1);
        BOOST_CHECK_EQUAL(pid.run(-1000), 999);
        BOOST_CHECK_EQUAL(pid.run(0), -1000);
    }

    BOOST_AUTO_TEST_CASE(
            pid_d_switch_coefficient,
            * boost::unit_test::depends_on("PID/pid_d")) {
        BOOST_TEST_MESSAGE("PID: Check derivative term (switch coefficient)");

        pid<float> pid{1};

        pid.set_d(1);
        pid.enable();

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 1);

        pid.set_d(2);

        BOOST_CHECK_EQUAL(pid.run(0), -2);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 2);

        pid.set_d(-1);

        BOOST_CHECK_EQUAL(pid.run(0), 1);
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), -1);
        BOOST_CHECK_EQUAL(pid.run(0), 1);
    }

    BOOST_AUTO_TEST_CASE(
            pid_d_gain,
            * boost::unit_test::depends_on("PID/pid_d")) {
        BOOST_TEST_MESSAGE("PID: Check gain for derivative term");

        pid<float> pid{1};

        pid.set_gain(5);
        pid.set_d(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 5);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.run(1000), 4995);
        BOOST_CHECK_EQUAL(pid.run(0), -5000);
        BOOST_CHECK_EQUAL(pid.run(-1), -5);
        BOOST_CHECK_EQUAL(pid.run(-1000), -4995);
    }

    BOOST_AUTO_TEST_CASE(
            pid_d_time,
            * boost::unit_test::depends_on("PID/pid_d")) {
        BOOST_TEST_MESSAGE("PID: Check timing for derivative term");

        pid<float> pid{0.001F};

        pid.set_d(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 0.001F);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_CLOSE_FRACTION(pid.run(0), 0, 0.000001F);
        BOOST_CHECK_CLOSE_FRACTION(pid.run(1), 1000, 0.000001F);
        BOOST_CHECK_CLOSE_FRACTION(pid.run(1000), 999000, 0.000001F);
        BOOST_CHECK_CLOSE_FRACTION(pid.run(0), -1000000, 0.000001F);
        BOOST_CHECK_CLOSE_FRACTION(pid.run(-1), -1000, 0.000001F);
        BOOST_CHECK_CLOSE_FRACTION(pid.run(-1000), -999000, 0.000001F);
    }

    BOOST_AUTO_TEST_CASE(
            pid_d_reset,
            * boost::unit_test::depends_on("PID/pid_d")) {
        BOOST_TEST_MESSAGE("PID: Check reset for derivative term");

        pid<float> pid{1};

        pid.set_d(1);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 0);
        BOOST_CHECK_EQUAL(pid.get_d(), 1);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(1), 1);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(1000), 1000);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(-1), -1);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(-1000), -1000);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_wo_anti_windup,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check for integral term without anti-windup");

        pid<float> pid{1};

        pid.set_i(1);
        pid.set_control_variable_minimum(-2);
        pid.set_control_variable_maximum(5);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);
        BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), -2);
        BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(5), 2.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(0), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-1), 4.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), 4);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-2), -1);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(0), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(1), -1.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), -1);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
    }

    BOOST_AUTO_TEST_CASE(
            pid_i_anti_windup_conditional_integration,
            * boost::unit_test::depends_on("PID/pid_i")) {
        BOOST_TEST_MESSAGE("PID: Check for integral term anti-windup (conditional integration)");

        pid<float, anti_windup::conditional_integration> pid{1};

        pid.set_i(1);
        pid.set_control_variable_minimum(-2);
        pid.set_control_variable_maximum(5);

        pid.enable();

        BOOST_CHECK_EQUAL(pid.get_state(), "running");
        BOOST_CHECK_EQUAL(pid.get_sampling_interval(), 1);
        BOOST_CHECK_EQUAL(pid.get_gain(), 1);
        BOOST_CHECK_EQUAL(pid.get_p(), 0);
        BOOST_CHECK_EQUAL(pid.get_i(), 1);
        BOOST_CHECK_EQUAL(pid.get_d(), 0);
        BOOST_CHECK_EQUAL(pid.get_control_variable_minimum(), -2);
        BOOST_CHECK_EQUAL(pid.get_control_variable_maximum(), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);

        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(5), 2.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(0), 5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-1), 4.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), 4);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        pid.reset();
        BOOST_CHECK_EQUAL(pid.run(0), 0);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-2), -1);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(-1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(1), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), true);
        BOOST_CHECK_EQUAL(pid.run(0), -2);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(1), -1.5);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
        BOOST_CHECK_EQUAL(pid.run(0), -1);
        BOOST_CHECK_EQUAL(pid.is_limited(), false);
    }

    BOOST_AUTO_TEST_CASE(pid_coefficient_plant, * tolerance(percent_tolerance(0.002))) {
        BOOST_TEST_MESSAGE("PID: Check the calculated plant coefficient");

        {
            auto [a, b] = toptica::plant<float, double>(
                0.001,
                100.0,
                0.5);
            BOOST_TEST(a.size() == 3);
            BOOST_TEST(a[0] == 1);
            BOOST_TEST(a[1] == -1.8953);
            BOOST_TEST(a[2] == 0.90484);
            BOOST_TEST(b.size() == 3);
            BOOST_TEST(b[0] == 0);
            BOOST_TEST(b[1] == 0.0048334);
            BOOST_TEST(b[2] == 0.0046749);
        }

        {
            auto [a, b] = toptica::plant<float, double>(
                0.001,
                10.0,
                0.1);
            BOOST_TEST(a.size() == 3);
            BOOST_TEST(a[0] == 1);
            BOOST_TEST(a[1] == -1.9979);
            BOOST_TEST(a[2] == 0.998);
            BOOST_TEST(b.size() == 3);
            BOOST_TEST(b[0] == 0);
            BOOST_TEST(b[1] == 0.000049966);
            BOOST_TEST(b[2] == 0.000049933);
        }
    }

    BOOST_AUTO_TEST_CASE(pid_closed_loop, * tolerance(percent_tolerance(0.002))) {
        BOOST_TEST_MESSAGE("PID: Check the closed loop response");

        toptica::tsp::iir::iir<double> plant{};
        pid<float> pid{
                0.001F,
                14.6F,
                6.0F,
                1.02F};

        {
            auto [a, b] = toptica::plant<double, double>(
                0.001,
                10.0,
                0.1);
            plant.set_coefficients(
                a,
                b);
        }

        double output{};

        pid.enable();

        for (auto& output_reference : toptica::test::data::pid_closed_loop_response) {
            output = plant.filter(static_cast<double>(
                        pid.run(static_cast<float>(1.0 - output))));
            BOOST_TEST(output == output_reference);
        }
    }
BOOST_AUTO_TEST_SUITE_END()
