/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2021
 *
 * @file        test_quadratic_fit.cpp
 * @brief       Unit Tests for the quadratic fit.
 *
 * @author      Euteneuer, Arno <Arno.Euteneuer@toptica.com>
 *
 ******************************************************************************/
#include <container/static_vector.hpp>
#include <tsp/fit_quadratic.hpp>
#include <boost/test/unit_test.hpp>
#include <limits>
#include <vector>

using namespace toptica::tsp::fit;
using namespace toptica::container;
using boost::test_tools::fpc::percent_tolerance;
using boost::unit_test::tolerance;

BOOST_AUTO_TEST_SUITE(QUADRATIC_FIT)

BOOST_AUTO_TEST_CASE(range_checks)
{
    using container = std::vector<double>;

    const container x_too_small{-1, 0};
    const container y_too_small{1, 0};
    const container x_ok{-1, 0, 1};
    const container y_ok{1, 0, 1};
    const container x_bigger{-2, -1, 0, 1, 2};
    const container y_bigger{4, 1, 0, 1, 4};
    container       c_too_small(2);
    container       c_ok(3);

    fit_quadratic<container, container> qfit;
    BOOST_CHECK_EQUAL(qfit.number_of_coeffs(), 3);

    BOOST_CHECK_EQUAL(qfit(x_ok, y_ok, c_ok), true);         // everything ok
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_bigger, c_ok), true); // everything ok

    BOOST_CHECK_EQUAL(qfit(x_ok, y_bigger, c_ok), false);           // different x and y sizes
    BOOST_CHECK_EQUAL(qfit(x_too_small, y_too_small, c_ok), false); // too small x and y sizes
    BOOST_CHECK_EQUAL(qfit(x_ok, y_ok, c_too_small), false);        // too small c size

    BOOST_CHECK_EQUAL(qfit(x_bigger, y_bigger, 0, 5, c_ok), true); // everything ok
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_ok, 0, 3, c_ok), true);     // everything ok

    BOOST_CHECK_EQUAL(qfit(x_bigger, y_ok, 0, 3, c_too_small), false); // too small c size
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_bigger, 1, 2, c_ok), false);    //  too small n
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_bigger, 10, 3, c_ok), false);   //  too to high pos
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_bigger, 2, 5, c_ok), false);    //  range exceeding container size
    BOOST_CHECK_EQUAL(qfit(x_bigger, y_ok, 0, 5, c_ok), false);        //  y size smaller than n
    BOOST_CHECK_EQUAL(qfit(x_ok, y_bigger, 0, 5, c_ok), false);        //  x size smaller than n
}

BOOST_AUTO_TEST_CASE(three_point_fit)
{
    using container = std::vector<double>;

    const container x{-1, 0, 1};
    const container y{1, 0, 1};
    container       c(3);

    fit_quadratic<container, container> qfit;
    BOOST_CHECK_EQUAL(qfit.number_of_coeffs(), 3);
    qfit.diagnosis = true;

    const bool result = qfit(x, y, c);
    BOOST_CHECK_EQUAL(result, true);
    BOOST_CHECK_EQUAL(c[0], 0);
    BOOST_CHECK_EQUAL(c[1], 0);
    BOOST_CHECK_EQUAL(c[2], 1);

    BOOST_CHECK_EQUAL(qfit.rss(), 0.);
    BOOST_CHECK_EQUAL(qfit.r_square(), 1.);
    BOOST_CHECK_EQUAL(qfit.rmse(), 0.);
}

BOOST_AUTO_TEST_CASE(multi_point_fit, *tolerance(percent_tolerance(0.5)))
{
    constexpr size_t n = 100;

    using d_container = std::array<float, n>;
    using c_container = std::array<float, 3>;

    constexpr auto x = [] {
        d_container _x{};
        float       xi = 0.f;
        for (size_t i = 0; i < n; xi += 0.1f, ++i) {
            _x.at(i) = xi;
        }
        return _x;
    }();

    constexpr auto y = [] {
        d_container _y{};
        float       xi = 0.f;
        for (size_t i = 0; i < n; xi += 0.1f, ++i) {
            _y.at(i) = 1.5f * xi * xi - 60.f * xi + 526.5f;
        }
        return _y;
    }();

    c_container c{};

    fit_quadratic<d_container, c_container> qfit;
    qfit.diagnosis = true;

    const bool result = qfit(x, y, n / 4, n / 2, c);
    BOOST_CHECK_EQUAL(result, true);
    BOOST_TEST(c[0] == 526.5);
    BOOST_TEST(c[1] == -60.);
    BOOST_TEST(c[2] == 1.5);

    BOOST_TEST(qfit.rss() == 1.17329335);
    BOOST_TEST(qfit.r_square() == 1.);
    BOOST_TEST(qfit.rmse() == 0.157999009);
}

BOOST_AUTO_TEST_CASE(multi_point_fit_static_vector, *tolerance(percent_tolerance(0.1)))
{
    constexpr size_t n = 20;

    using d_container = static_vector<float, n>;
    using c_container = static_vector<float, 3>;

    const auto x = [] {
        d_container _x{};
        for (float i = 0; i < n; i += 1) {
            _x.push_back(i);
        }
        return _x;
    }();

    const auto y = [] {
        d_container _y{};
        for (float i = 0; i < n; i += 1) {
            _y.push_back(-2.F * i * i + 0.5F * i - 1.5F);
        }
        return _y;
    }();

    c_container c{0.F, 0.F, 0.F};

    fit_quadratic<d_container, c_container> qfit;
    qfit.diagnosis = true;

    const bool result = qfit(x, y, c);
    BOOST_CHECK_EQUAL(result, true);
    BOOST_TEST(c[0] == -1.5);
    BOOST_TEST(c[1] == 0.5);
    BOOST_TEST(c[2] == -2.0);

    BOOST_TEST(qfit.rss() == 0.0006579);
    BOOST_TEST(qfit.r_square() == 1.);
    BOOST_TEST(qfit.rmse() == 0.0062213);
}

BOOST_AUTO_TEST_SUITE_END()
