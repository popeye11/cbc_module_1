/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        test_data.hpp
 * @brief       Test data for TOPTICA TSP unit tests.
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
#pragma once

#include <array>

namespace toptica::test::data {

extern std::array<float, 1000> butterworth_2_order_low_pass_1_500_impulse_response;
extern std::array<double, 301> pid_closed_loop_response;

}  // namespace toptica::test::data
