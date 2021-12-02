/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2021
 *
 * @file        test_ransac.cpp
 * @brief       Unit Tests for the RANSAC fitter.
 *
 * @author      Euteneuer, Arno <Arno.Euteneuer@toptica.com>
 *
 ******************************************************************************/
#include <container/static_vector.hpp>
#include <tsp/fit_quadratic.hpp>
#include <tsp/fit_ransac.hpp>
#include <boost/test/unit_test.hpp>
#include <chrono>
#include <limits>
#include <vector>

using namespace toptica::tsp::fit;
using namespace toptica::container;
using boost::test_tools::fpc::percent_tolerance;
using boost::unit_test::tolerance;

BOOST_AUTO_TEST_SUITE(RANSAC_FIT)

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
    fit_ransac                       ransac(qfit);

    BOOST_CHECK_EQUAL(ransac(x_ok, y_ok, c_ok, 3, 0.5), true);         // everything ok
    BOOST_CHECK_EQUAL(ransac(x_bigger, y_bigger, c_ok, 3, 0.5), true); // everything ok

    BOOST_CHECK_EQUAL(ransac(x_ok, y_bigger, c_ok, 3, 0.5), false);           // different x and y sizes
    BOOST_CHECK_EQUAL(ransac(x_too_small, y_too_small, c_ok, 3, 0.5), false); // too small x and y sizes
    BOOST_CHECK_EQUAL(ransac(x_ok, y_ok, c_too_small, 3, 0.5), false);        // too small c size
    BOOST_CHECK_EQUAL(ransac(x_bigger, y_bigger, c_ok, 2, 0.5), false);       // too small inlier count
}

BOOST_AUTO_TEST_CASE(no_outliers, *tolerance(percent_tolerance(0.2)))
{
    constexpr size_t n = 100;

    using d_container = std::array<double, n>;
    using c_container = std::array<double, 3>;

    constexpr auto x = [] {
        d_container _x{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {
            _x.at(i) = xi;
        }
        return _x;
    }();

    constexpr auto y = [] {
        d_container _y{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {
            _y.at(i) = 1.5 * xi * xi - 60. * xi + 526.5;
        }
        return _y;
    }();

    c_container c_fit{{0., 0., 0.}};
    c_container c_ransac{{0., 0., 0.}};

    fit_quadratic<d_container, c_container> qfit;
    fit_ransac                           ransac(qfit);

    const bool fit_result    = qfit(x, y, c_fit);
    const bool ransac_result = ransac(x, y, c_ransac, n / 2, 5.0);

    BOOST_CHECK_EQUAL(fit_result, true);
    BOOST_CHECK_EQUAL(ransac_result, true);
    BOOST_CHECK_EQUAL(ransac.number_of_inliers(), 100);

    BOOST_TEST(c_fit[0] == c_ransac[0]);
    BOOST_TEST(c_fit[1] == c_ransac[1]);
    BOOST_TEST(c_fit[2] == c_ransac[2]);

    BOOST_TEST(ransac.rss() == 0.);
    BOOST_TEST(ransac.r_square() == 1.);
    BOOST_TEST(ransac.rmse() == 0.);
}

BOOST_AUTO_TEST_CASE(twenty_percent_outliers, *tolerance(percent_tolerance(0.2)))
{
    constexpr size_t n = 100;

    using d_container = std::array<double, n>;
    using c_container = std::array<double, 3>;

    constexpr auto x = [] {
        d_container _x{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {
            _x.at(i) = xi;
        }
        return _x;
    }();

    auto y = [] {
        d_container _y{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {

            if ((i % 5) == 1) {

                _y.at(i) = -1. * xi * xi * xi + 2 * xi * xi; // outliers
            } else {
                _y.at(i) = 1.5 * xi * xi - 60. * xi + 526.5;
            }
        }
        return _y;
    }();

    c_container c_fit{{0., 0., 0.}};
    c_container c_ransac{{0., 0., 0.}};

    fit_quadratic<d_container, c_container> qfit;
    fit_ransac                           ransac(qfit);

    const bool fit_result    = qfit(x, y, c_fit);
    const bool ransac_result = ransac(x, y, c_ransac, n / 2, 5.0);

    BOOST_CHECK_EQUAL(fit_result, true);
    BOOST_CHECK_EQUAL(ransac_result, true);
    BOOST_CHECK_EQUAL(ransac.number_of_inliers(), 80);

    BOOST_TEST(c_ransac[0] == 526.5);
    BOOST_TEST(c_ransac[1] == -60.);
    BOOST_TEST(c_ransac[2] == 1.5);

    BOOST_TEST(c_fit[0] != c_ransac[0]);
    BOOST_TEST(c_fit[1] != c_ransac[1]);
    BOOST_TEST(c_fit[2] != c_ransac[2]);

    BOOST_TEST(ransac.rss() == 0.);
    BOOST_TEST(ransac.r_square() == 1.);
    BOOST_TEST(ransac.rmse() == 0.);
}

BOOST_AUTO_TEST_CASE(twenty_percent_outliers_static_vector, *tolerance(percent_tolerance(0.2)))
{
    constexpr size_t n = 100;

    using d_container = static_vector<double, n>;
    using c_container = static_vector<double, 3>;
    using i_container = static_vector<size_t, 3>;

    const auto x = [] {
        d_container _x{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {
            _x.push_back(xi);
        }
        return _x;
    }();

    const auto y = [] {
        d_container _y{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {

            if ((i % 5) == 1) {

                _y.push_back(-1. * xi * xi * xi + 2 * xi * xi); // outliers
            } else {
                _y.push_back(1.5 * xi * xi - 60. * xi + 526.5);
            }
        }
        return _y;
    }();

    c_container c_fit{0., 0., 0.};
    c_container c_ransac{0., 0., 0.};

    fit_quadratic<d_container, c_container>              qfit;
    fit_ransac<d_container, c_container, i_container> ransac(qfit);

    const bool fit_result    = qfit(x, y, c_fit);
    const bool ransac_result = ransac(x, y, c_ransac, n / 2, 5.0);

    BOOST_CHECK_EQUAL(fit_result, true);
    BOOST_CHECK_EQUAL(ransac_result, true);
    BOOST_CHECK_EQUAL(ransac.number_of_inliers(), 80);

    BOOST_TEST(c_ransac[0] == 526.5);
    BOOST_TEST(c_ransac[1] == -60.);
    BOOST_TEST(c_ransac[2] == 1.5);

    BOOST_TEST(c_fit[0] != c_ransac[0]);
    BOOST_TEST(c_fit[1] != c_ransac[1]);
    BOOST_TEST(c_fit[2] != c_ransac[2]);

    BOOST_TEST(ransac.rss() == 0.);
    BOOST_TEST(ransac.r_square() == 1.);
    BOOST_TEST(ransac.rmse() == 0.);
}

BOOST_AUTO_TEST_CASE(twenty_percent_outliers_std_vector, *tolerance(percent_tolerance(0.2)))
{
    constexpr size_t n = 100;

    using d_container = std::vector<double>;
    using c_container = std::vector<double>;

    const auto x = [] {
        d_container _x{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {
            _x.push_back(xi);
        }
        return _x;
    }();

    const auto y = [] {
        d_container _y{};
        double      xi = 0;
        for (size_t i = 0; i < n; xi += 0.1, ++i) {

            if ((i % 5) == 1) {

                _y.push_back(-1. * xi * xi * xi + 2 * xi * xi); // outliers
            } else {
                _y.push_back(1.5 * xi * xi - 60. * xi + 526.5);
            }
        }
        return _y;
    }();

    c_container c_fit{{0., 0., 0.}};
    c_container c_ransac{{0., 0., 0.}};

    fit_quadratic<d_container, c_container> qfit;
    fit_ransac                              ransac(qfit);

    const bool fit_result    = qfit(x, y, c_fit);
    const bool ransac_result = ransac(x, y, c_ransac, n / 2, 5.0);

    BOOST_CHECK_EQUAL(fit_result, true);
    BOOST_CHECK_EQUAL(ransac_result, true);
    BOOST_CHECK_EQUAL(ransac.number_of_inliers(), 80);

    BOOST_TEST(c_ransac[0] == 526.5);
    BOOST_TEST(c_ransac[1] == -60.);
    BOOST_TEST(c_ransac[2] == 1.5);

    BOOST_TEST(c_fit[0] != c_ransac[0]);
    BOOST_TEST(c_fit[1] != c_ransac[1]);
    BOOST_TEST(c_fit[2] != c_ransac[2]);

    BOOST_TEST(ransac.rss() == 0.);
    BOOST_TEST(ransac.r_square() == 1.);
    BOOST_TEST(ransac.rmse() == 0.);
}

BOOST_AUTO_TEST_SUITE_END()
