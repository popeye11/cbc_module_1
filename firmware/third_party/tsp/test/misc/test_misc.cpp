/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2019-2020
 *
 * @file        test_misc.cpp
 * @brief       Unit Tests for various features of the TSP library.
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
#include <version.hpp>

BOOST_AUTO_TEST_SUITE(PID)

    BOOST_AUTO_TEST_CASE(version) {
        BOOST_TEST_MESSAGE("Misc: Check Version String");

        std::string ver = g_version.data();

        BOOST_TEST(!ver.empty());
        BOOST_TEST(ver.length() >= 7); // Git hash is at least 7 chars long
    }

BOOST_AUTO_TEST_SUITE_END()
