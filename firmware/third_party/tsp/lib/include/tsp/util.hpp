/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        util.hpp
 * @brief       Helper function for the TSP library.
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

#include <tsp/filter.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <tuple>
#include <vector>

namespace toptica::tsp {

template<
    typename T,
    template<class, class> typename Vector = std::vector,
    template<class> typename Allocator = std::allocator>
constexpr std::tuple<
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    std::complex<T>> bilinear(
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& zeros,
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& poles,
        const std::complex<T>& gain);

template<
    typename T,
    template<class, class> typename Vector = std::vector,
    template<class> typename Allocator = std::allocator>
constexpr std::tuple<
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    std::complex<T>> sft(
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& zeros,
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& poles,
        const std::complex<T>& gain,
        T frequency,
        filter::type type);

template<
    typename T,
    typename U,
    template<class, class> typename Vector = std::vector,
    template<class> typename Allocator = std::allocator>
constexpr std::tuple<
    Vector<T, Allocator<T>>,
    Vector<T, Allocator<T>>> zp2h(
        const Vector<std::complex<U>, Allocator<std::complex<U>>>& zeros,
        const Vector<std::complex<U>, Allocator<std::complex<U>>>& poles,
        const std::complex<U>& gain);

/*******************************************************************************
 * @brief                   Converts the s-domain transfer function in pole-zero
 *                          form specified by poles, zeros and gain to a
 *                          discrete equivalent.
 *
 * @param zeros             The zeros.
 * @param poles             The poles.
 * @param gain              The gain.
 * @return                  Tuple of zeros, poles and gain.
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr std::tuple<
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    std::complex<T>> bilinear(
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& zeros,
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& poles,
        const std::complex<T>& gain) {
    const std::array<std::complex<T>, 2> numerator({{-2, 2}});
    const std::array<std::complex<T>, 2> denominator({{1, 1}});
    Vector<std::complex<T>, Allocator<std::complex<T>>> _zeros{};
    Vector<std::complex<T>, Allocator<std::complex<T>>> _poles{};
    std::complex<T> _gain{gain};

    for (auto& zero : zeros) {
        auto _numerator{numerator[0] - (denominator[0] * zero)};
        auto _denominator{numerator[1] - (denominator[1] * zero)};
        _zeros.push_back(- _numerator / _denominator);
        _gain *= _denominator;
    }
    for (auto& pole : poles) {
        auto _numerator{numerator[0] - (denominator[0] * pole)};
        auto _denominator{numerator[1] - (denominator[1] * pole)};

        _poles.push_back(- _numerator / _denominator);
        _gain /= _denominator;
    }
    while (_poles.size() > _zeros.size()) {
        _zeros.push_back(- denominator[0] / denominator[1]);
        _gain *= denominator[1];
    }
    while (_zeros.size() > _poles.size()) {
        _poles.push_back(- denominator[0] / denominator[1]);
        _gain /= denominator[1];
    }

    return std::make_tuple(
        _zeros,
        _poles,
        _gain);
}

/*******************************************************************************
 * @brief                   s-plane frequency transform.
 *
 * @param zeros             The zeros.
 * @param poles             The poles.
 * @param gain              The gain.
 * @param frequency         The frequency.
 * @param type              The type of the filter (low-pass, high-pass,
 *                          band-pass, band-stop).
 * @return                  Tuple of zeros, poles and gain.
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr std::tuple<
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    Vector<std::complex<T>, Allocator<std::complex<T>>>,
    std::complex<T>> sft(
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& zeros,
        const Vector<std::complex<T>, Allocator<std::complex<T>>>& poles,
        const std::complex<T>& gain,
        const T frequency,
        const filter::type type) {
    Vector<std::complex<T>, Allocator<std::complex<T>>> _zeros{};
    Vector<std::complex<T>, Allocator<std::complex<T>>> _poles{};
    std::complex<T> _gain{gain};

    assert((type == filter::type::low_pass) || (type == filter::type::high_pass)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    switch (type) {
        case filter::type::low_pass:
            for (auto& zero : zeros) {
                _zeros.push_back(zero * frequency);
            }
            for (auto& pole : poles) {
                _poles.push_back(pole * frequency);
            }
            _gain *= std::pow(frequency, poles.size() - zeros.size());
            break;
        case filter::type::high_pass:
            for (auto& zero : zeros) {
                _zeros.push_back(frequency / zero);
                _gain /= -zero;
            }
            for (auto& pole : poles) {
                _poles.push_back(frequency / pole);
                _gain /= -pole;
            }
            while (_poles.size() > _zeros.size()) {
                _zeros.push_back(std::complex<T>(0));
            }
            break;
    }

    return std::make_tuple(
        _zeros,
        _poles,
        _gain);
}

/*******************************************************************************
 * @brief                   convert zeros and poles to coefficients.
 *
 * @param zeros             The zeros.
 * @param poles             The poles.
 * @param gain              The gain.
 * @return                  Tuple of a- and b-coefficients.
 ******************************************************************************/
template<
    typename T,
    typename U,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr std::tuple<
    Vector<T, Allocator<T>>,
    Vector<T, Allocator<T>>> zp2h(
        const Vector<std::complex<U>, Allocator<std::complex<U>>>& zeros,
        const Vector<std::complex<U>, Allocator<std::complex<U>>>& poles,
        const std::complex<U>& gain) {
    auto order{zeros.size()};
    Vector<std::complex<U>, Allocator<std::complex<U>>> a_complex(order + 1);
    Vector<std::complex<U>, Allocator<std::complex<U>>> b_complex(order + 1);
    Vector<T, Allocator<T>> a(order + 1);
    Vector<T, Allocator<T>> b(order + 1);

    a_complex[0] = 1;
    b_complex[0] = 1;

    for (std::size_t _i = 0; _i < order; ++_i) {
        for (std::size_t _j = 0; _j <= _i; ++_j) {
            a_complex[_i - _j + 1U] -= poles[_i] * a_complex[_i - _j];
            b_complex[_i - _j + 1U] -= zeros[_i] * b_complex[_i - _j];
        }
    }
    for (auto& _b : b_complex) {
        _b *= gain;
    }

    std::transform(
        a_complex.begin(),
        a_complex.end(),
        a.begin(),
        [](std::complex<U> value) {return static_cast<T>(value.real());});
    std::transform(
        b_complex.begin(),
        b_complex.end(),
        b.begin(),
        [](std::complex<U> value) {return static_cast<T>(value.real());});

    return std::make_tuple(
        a,
        b);
}

}  // namespace toptica::tsp
