/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        test_iir.cpp
 * @brief       Unit Tests for the Infinite Impulse Response filter.
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

#include <array>
#include <vector>

#include <test_data.hpp>

#include <tsp/iir.hpp>

using namespace toptica::tsp;

BOOST_AUTO_TEST_SUITE(iir)
    BOOST_AUTO_TEST_CASE(iir_default_constructor) {
        BOOST_TEST_MESSAGE("iir: Default constructor");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 0);
        BOOST_TEST_CHECK(b.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(iir_from_coefficients_unmatched) {
        BOOST_TEST_MESSAGE("iir: Constructor with unmatched number of coefficients");

        {
            // Instantiate the filter
            toptica::tsp::iir::iir<float> iir{
                std::vector<float>{1.0F, 2.0F, 3.0F, 4.0F},
                std::vector<float>{5.0F, 6.0F, 7.0F}};

            // get the coefficients
            auto [a, b] = iir.get_coefficients();
            BOOST_TEST_CHECK(a.size() == 0);
            BOOST_TEST_CHECK(b.size() == 0);
        }
        {
            // Instantiate the filter
            toptica::tsp::iir::iir<float> iir{
                std::vector<float>{1.0F, 2.0F, 3.0F},
                std::vector<float>{4.0F, 5.0F, 6.0F, 7.0F}};

            // get the coefficients
            auto [a, b] = iir.get_coefficients();
            BOOST_TEST_CHECK(a.size() == 0);
            BOOST_TEST_CHECK(b.size() == 0);
        }
    }

    BOOST_AUTO_TEST_CASE(iir_from_coefficients_matched) {
        BOOST_TEST_MESSAGE("iir: Constructor with matched number of coefficients");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            std::vector<float>{1.0F, 2.0F, 3.0F, 4.0F},
            std::vector<float>{5.0F, 6.0F, 7.0F, 8.0F}};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 2.0F);
        BOOST_TEST_CHECK(a[2] == 3.0F);
        BOOST_TEST_CHECK(a[3] == 4.0F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 5.0F);
        BOOST_TEST_CHECK(b[1] == 6.0F);
        BOOST_TEST_CHECK(b[2] == 7.0F);
        BOOST_TEST_CHECK(b[3] == 8.0F);
    }



    BOOST_AUTO_TEST_CASE(iir_set_coefficients_unmatched) {
        BOOST_TEST_MESSAGE("iir: Set unmatched number of coefficients");

        {
            // Instantiate the filter
            toptica::tsp::iir::iir<float> iir{};

            iir.set_coefficients(
                std::vector<float>{1.0F, 2.0F, 3.0F, 4.0F},
                std::vector<float>{5.0F, 6.0F, 7.0F});

            // get the coefficients
            auto [a, b] = iir.get_coefficients();
            BOOST_TEST_CHECK(a.size() == 0);
            BOOST_TEST_CHECK(b.size() == 0);
        }
        {
            // Instantiate the filter
            toptica::tsp::iir::iir<float> iir{};

            iir.set_coefficients(
                std::vector<float>{1.0F, 2.0F, 3.0F},
                std::vector<float>{4.0F, 5.0F, 6.0F, 7.0F});

            // get the coefficients
            auto [a, b] = iir.get_coefficients();
            BOOST_TEST_CHECK(a.size() == 0);
            BOOST_TEST_CHECK(b.size() == 0);
        }
    }

    BOOST_AUTO_TEST_CASE(iir_set_coefficients_matched) {
        BOOST_TEST_MESSAGE("iir: Set matched number of coefficients");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{};

        iir.set_coefficients(
            std::vector<float>{1.0F, 2.0F, 3.0F, 4.0F},
            std::vector<float>{5.0F, 6.0F, 7.0F, 8.0F});

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 2.0F);
        BOOST_TEST_CHECK(a[2] == 3.0F);
        BOOST_TEST_CHECK(a[3] == 4.0F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 5.0F);
        BOOST_TEST_CHECK(b[1] == 6.0F);
        BOOST_TEST_CHECK(b[2] == 7.0F);
        BOOST_TEST_CHECK(b[3] == 8.0F);
    }


    BOOST_AUTO_TEST_CASE(iir_butterworth_2_order_low_pass_1_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "2. Order Butterworth low pass filter with fc=1/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            1.0F/500.0F,
            toptica::tsp::filter::type::low_pass,
            2,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 3);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == -1.98222888F);
        BOOST_TEST_CHECK(a[2] == 0.982385457F);
        BOOST_TEST_CHECK(b.size() == 3);
        BOOST_TEST_CHECK(b[0] == 3.91302092e-05F);
        BOOST_TEST_CHECK(b[1] == 7.82604184e-05F);
        BOOST_TEST_CHECK(b[2] == 3.91302092e-05F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_2_order_low_pass_249_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "2. Order Butterworth low pass filter with fc=249/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            249.0F/500.0F,
            toptica::tsp::filter::type::low_pass,
            2,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 3);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 1.98222888F);
        BOOST_TEST_CHECK(a[2] == 0.982385397F);
        BOOST_TEST_CHECK(b.size() == 3);
        BOOST_TEST_CHECK(b[0] == 0.991153598F);
        BOOST_TEST_CHECK(b[1] == 1.9823072F);
        BOOST_TEST_CHECK(b[2] == 0.991153598F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_2_order_high_pass_1_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "2. Order Butterworth high pass filter with fc=1/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            1.0F/500.0F,
            toptica::tsp::filter::type::high_pass,
            2,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 3);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == -1.98222888F);
        BOOST_TEST_CHECK(a[2] == 0.982385457F);
        BOOST_TEST_CHECK(b.size() == 3);
        BOOST_TEST_CHECK(b[0] == 0.991153598F);
        BOOST_TEST_CHECK(b[1] == -1.9823072F);
        BOOST_TEST_CHECK(b[2] == 0.991153598F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_2_order_high_pass_249_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "2. Order Butterworth high pass filter with fc=249/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            249.0F/500.0F,
            toptica::tsp::filter::type::high_pass,
            2,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 3);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 1.98222888F);
        BOOST_TEST_CHECK(a[2] == 0.982385397F);
        BOOST_TEST_CHECK(b.size() == 3);
        BOOST_TEST_CHECK(b[0] == 3.9130362e-05F);
        BOOST_TEST_CHECK(b[1] == -7.8260724e-05F);
        BOOST_TEST_CHECK(b[2] == 3.9130362e-05F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_3_order_low_pass_1_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "3. Order Butterworth low pass filter with fc=1/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            1.0F/500.0F,
            toptica::tsp::filter::type::low_pass,
            3,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == -2.97486734F);
        BOOST_TEST_CHECK(a[2] == 2.95004964F);
        BOOST_TEST_CHECK(a[3] == -0.975180268F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 2.44962308e-07F);
        BOOST_TEST_CHECK(b[1] == 7.34886953e-07F);
        BOOST_TEST_CHECK(b[2] == 7.34886953e-07F);
        BOOST_TEST_CHECK(b[3] == 2.44962308e-07F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_3_order_low_pass_249_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "3. Order Butterworth low pass filter with fc=249/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            249.0F/500.0F,
            toptica::tsp::filter::type::low_pass,
            3,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 2.97486734F);
        BOOST_TEST_CHECK(a[2] == 2.95004964F);
        BOOST_TEST_CHECK(a[3] == 0.975180268F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 0.987512171F);
        BOOST_TEST_CHECK(b[1] == 2.96253633F);
        BOOST_TEST_CHECK(b[2] == 2.96253633F);
        BOOST_TEST_CHECK(b[3] == 0.987512171F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_3_order_high_pass_1_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "3. Order Butterworth high pass filter with fc=1/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            1.0F/500.0F,
            toptica::tsp::filter::type::high_pass,
            3,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == -2.97486734F);
        BOOST_TEST_CHECK(a[2] == 2.95004964F);
        BOOST_TEST_CHECK(a[3] == -0.975180268F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 0.987512171F);
        BOOST_TEST_CHECK(b[1] == -2.96253657F);
        BOOST_TEST_CHECK(b[2] == 2.96253657F);
        BOOST_TEST_CHECK(b[3] == -0.987512171F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_3_order_high_pass_249_500) {
        BOOST_TEST_MESSAGE("iir: Coefficients for "
            "3. Order Butterworth high pass filter with fc=249/500fs");

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            249.0F/500.0F,
            toptica::tsp::filter::type::high_pass,
            3,
            toptica::tsp::iir::characteristic::butterworth};

        // get the coefficients
        auto [a, b] = iir.get_coefficients();
        BOOST_TEST_CHECK(a.size() == 4);
        BOOST_TEST_CHECK(a[0] == 1.0F);
        BOOST_TEST_CHECK(a[1] == 2.97486734F);
        BOOST_TEST_CHECK(a[2] == 2.95004964F);
        BOOST_TEST_CHECK(a[3] == 0.975180268F);
        BOOST_TEST_CHECK(b.size() == 4);
        BOOST_TEST_CHECK(b[0] == 2.44963758e-07F);
        BOOST_TEST_CHECK(b[1] == -7.34891273e-07F);
        BOOST_TEST_CHECK(b[2] == 7.34891273e-07F);
        BOOST_TEST_CHECK(b[3] == -2.44963758e-07F);
    }

    BOOST_AUTO_TEST_CASE(iir_butterworth_2_order_low_pass_1_500_impulse_response) {
        BOOST_TEST_MESSAGE("iir: impulse response for "
            "2. Order Butterworth low pass filter with fc=1/500fs");

        float x{1.0F};

        // Instantiate the filter
        toptica::tsp::iir::iir<float> iir{
            1.0F/500.0F,
            toptica::tsp::filter::type::low_pass,
            2,
            toptica::tsp::iir::characteristic::butterworth};

        for (auto& y : toptica::test::data::butterworth_2_order_low_pass_1_500_impulse_response) {
            BOOST_TEST_CHECK(y == iir.filter(x));
            x = 0.0F;
        }
    }

BOOST_AUTO_TEST_SUITE_END()
