
/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2020
 *
 * @file        plant.hpp
 * @brief       Calculate coefficients for plant with 2nd order transfer
 *              function and time delay.
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

#include <cmath>
#include <complex>
#include <tuple>
#include <vector>

namespace toptica {

template<typename T, typename U>
std::tuple<std::vector<T>, std::vector<T>> plant(
        const std::complex<U> Ts,
        const std::complex<U> omega,
        const std::complex<U> eta) {
    using namespace std;

    auto s1{eta * omega + sqrt((pow(eta, std::complex<U>(2)) - std::complex<U>(1)) * pow(omega, std::complex<U>(2)))};
    auto s2{eta * omega - sqrt((pow(eta, std::complex<U>(2)) - std::complex<U>(1)) * pow(omega, std::complex<U>(2)))};

    auto a0{
        - pow(s1, std::complex<U>(3)) * s2 * exp(Ts * s1) * exp(Ts * s2)
        + std::complex<U>(2) * pow(s1, std::complex<U>(2)) * pow(s2, std::complex<U>(2)) * exp(Ts * s1) * exp(Ts * s2)
        - s1 * pow(s2, std::complex<U>(3)) * exp(Ts * s1) * exp(Ts * s2)};

    auto a1{
        pow(s1, std::complex<U>(3)) * s2 * exp(Ts * s1)
        + pow(s1, std::complex<U>(3)) * s2 * exp(Ts * s2)
        - std::complex<U>(2) * pow(s1, std::complex<U>(2)) * pow(s2, std::complex<U>(2)) * exp(Ts * s1)
        - std::complex<U>(2) * pow(s1, std::complex<U>(2)) * pow(s2, std::complex<U>(2)) * exp(Ts * s2)
        + s1 * pow(s2, std::complex<U>(3)) * exp(Ts * s1)
        + s1 * pow(s2, std::complex<U>(3)) * exp(Ts * s2)};

    auto a2{
        - pow(s1, std::complex<U>(3)) * s2
        + std::complex<U>(2) * pow(s1, std::complex<U>(2)) * pow(s2, std::complex<U>(2))
        - s1 * pow(s2, std::complex<U>(3))};

    auto b1{
        - pow(s1, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s1) * exp(Ts * s2)
        + pow(s1, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s1)
        + std::complex<U>(2) * s1 * s2 * pow(omega, std::complex<U>(2)) * exp(Ts * s1) * exp(Ts * s2)
        - s1 * s2 * pow(omega, std::complex<U>(2)) * exp(Ts * s1)
        - s1 * s2 * pow(omega, std::complex<U>(2)) * exp(Ts * s2)
        - pow(s2, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s1) * exp(Ts * s2)
        + pow(s2, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s2)};

    auto b2{
        pow(s1, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s2)
        - pow(s1, std::complex<U>(2)) * pow(omega, std::complex<U>(2))
        - s1 * s2 * pow(omega, std::complex<U>(2)) * exp(Ts * s1)
        - s1 * s2 * pow(omega, std::complex<U>(2)) * exp(Ts * s2)
        + std::complex<U>(2) * s1 * s2 * pow(omega, std::complex<U>(2))
        + pow(s2, std::complex<U>(2)) * pow(omega, std::complex<U>(2)) * exp(Ts * s1)
        - pow(s2, std::complex<U>(2)) * pow(omega, std::complex<U>(2))};

    return std::make_tuple(
        std::vector<T>{{
            static_cast<T>(1),
            static_cast<T>((a1 / a0).real()),
            static_cast<T>((a2 / a0).real())}},
        std::vector<T>{{
            static_cast<T>(0),
            static_cast<T>((b1 / a0).real()),
            static_cast<T>((b2 / a0).real())}});
}

}  // namespace toptica