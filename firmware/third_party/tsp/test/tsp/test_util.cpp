/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        test_util.cpp
 * @brief       Unit Tests for the TSP utils
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

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <tuple>
#include <vector>

#include <tsp/util.hpp>

using namespace toptica::tsp;
using TestVector = std::vector<std::complex<double>,
                   std::allocator<std::complex<double>>>;
using TestScalar = std::complex<double>;

BOOST_AUTO_TEST_SUITE(util)
    BOOST_AUTO_TEST_CASE(bilinear_equal) {
        BOOST_TEST_MESSAGE("util: bilinear equal size");

        TestVector poles{1,-2};
        TestVector zeros{3,-3};
        TestScalar gain{1};

        std::tie(zeros, poles, gain) = bilinear<double,
                                                std::vector,
                                                std::allocator>(
                                                    zeros, poles, gain);

        BOOST_TEST_CHECK(zeros[0].real() == -5.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -0.2);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == 0.0);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == -1.25);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(bilinear_more_poles) {
        BOOST_TEST_MESSAGE("util: bilinear more poles");

        TestVector poles{1,-2, 1};
        TestVector zeros{3,-3};
        TestScalar gain{1};

        std::tie(zeros, poles, gain) = bilinear<double,
                                                std::vector,
                                                std::allocator>(
                                                    zeros, poles, gain);

        BOOST_TEST_CHECK(zeros[0].real() == -5.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -0.2);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[2].real() == -1.0);
        BOOST_TEST_CHECK(zeros[2].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == 0.0);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[2].real() == 3.0);
        BOOST_TEST_CHECK(poles[2].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == -1.25);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(bilinear_more_zeros) {
        BOOST_TEST_MESSAGE("util: bilinear more zeros");

        TestVector poles{1,-2};
        TestVector zeros{3,-3, 1};
        TestScalar gain{1};

        std::tie(zeros, poles, gain) = bilinear<double,
                                                std::vector,
                                                std::allocator>(
                                                    zeros, poles, gain);

        BOOST_TEST_CHECK(zeros[0].real() == -5.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -0.2);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[2].real() == 3.0);
        BOOST_TEST_CHECK(zeros[2].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == 0.0);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[2].real() == -1.0);
        BOOST_TEST_CHECK(poles[2].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == -1.25);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(sft_low_pass) {
        BOOST_TEST_MESSAGE("util: sft low pass");

        TestVector poles{1,-2};
        TestVector zeros{3,-3};
        TestScalar gain{1};
        double frequency = 3.0;

        std::tie(zeros, poles, gain) = sft<double,
                                           std::vector,
                                           std::allocator>(
                                               zeros,
                                               poles,
                                               gain,
                                               frequency,
                                               filter::type::low_pass);

        BOOST_TEST_CHECK(zeros[0].real() == 9.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -9.0);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == -6.0);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == 1.0);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(sft_high_pass, * boost::unit_test::tolerance(0.0001)) {
        BOOST_TEST_MESSAGE("util: sft high pass");

        TestVector poles{1,-2};
        TestVector zeros{3,-3};
        TestScalar gain{1};
        double frequency = 3.0;

        std::tie(zeros, poles, gain) = sft<double,
                                           std::vector,
                                           std::allocator>(
                                               zeros,
                                               poles,
                                               gain,
                                               frequency,
                                               filter::type::high_pass);

        BOOST_TEST_CHECK(zeros[0].real() == 1.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -1.0);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == -1.5);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == 0.0555556);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(sft_high_pass_more_poles, * boost::unit_test::tolerance(0.0001)) {
        BOOST_TEST_MESSAGE("util: sft high pass more poles");

        TestVector poles{1,-2, 3};
        TestVector zeros{3,-3};
        TestScalar gain{1};
        double frequency = 3.0;

        std::tie(zeros, poles, gain) = sft<double,
                                           std::vector,
                                           std::allocator>(
                                               zeros,
                                               poles,
                                               gain,
                                               frequency,
                                               filter::type::high_pass);

        BOOST_TEST_CHECK(zeros[0].real() == 1.0);
        BOOST_TEST_CHECK(zeros[0].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[1].real() == -1.0);
        BOOST_TEST_CHECK(zeros[1].imag() == 0.0);
        BOOST_TEST_CHECK(zeros[2].real() == 0.0);
        BOOST_TEST_CHECK(zeros[2].imag() == 0.0);
        BOOST_TEST_CHECK(poles[0].real() == 3.0);
        BOOST_TEST_CHECK(poles[0].imag() == 0.0);
        BOOST_TEST_CHECK(poles[1].real() == -1.5);
        BOOST_TEST_CHECK(poles[1].imag() == 0.0);
        BOOST_TEST_CHECK(poles[2].real() == 1.0);
        BOOST_TEST_CHECK(poles[2].imag() == 0.0);
        BOOST_TEST_CHECK(gain.real() == -0.0185185);
        BOOST_TEST_CHECK(gain.imag() == 0);
    }

    BOOST_AUTO_TEST_CASE(zp2h_normal) {
        BOOST_TEST_MESSAGE("util: zp2h");

        TestVector poles{1,-2, 3};
        TestVector zeros{3,-3};
        TestScalar gain{1};
        std::vector<double, std::allocator<double>> a{};
        std::vector<double, std::allocator<double>> b{};

        std::tie(a, b) = zp2h<double, double, std::vector, std::allocator>(
                                zeros,
                                poles,
                                gain);

        BOOST_TEST_CHECK(a[0] == 1.0);
        BOOST_TEST_CHECK(a[1] == 1.0);
        BOOST_TEST_CHECK(a[2] == -2.0);
        BOOST_TEST_CHECK(b[0] == 1.0);
        BOOST_TEST_CHECK(b[1] == 0.0);
        BOOST_TEST_CHECK(b[2] == -9.0);
    }

BOOST_AUTO_TEST_SUITE_END()
