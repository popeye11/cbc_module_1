/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        iir.hpp
 * @brief       Direct Form 2 Infinite Impulse Response filter.
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

#define _USE_MATH_DEFINES

#include <tsp/filter.hpp>
#include <tsp/util.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <tuple>
#include <vector>

namespace toptica::tsp::iir {

enum class characteristic {
    butterworth,
    // chebyshev,
    // bessel,
    // elliptic,
    // linkwitz_riley
};

template<
    typename T,
    template<class, class> typename Vector = std::vector,
    template<class> typename Allocator = std::allocator>
class iir {
  public:
    iir() = default;
    virtual ~iir()= default;
    iir(const iir&) = delete;
    iir& operator=(const iir&) = delete;
    iir(iir&&) = delete;
    iir& operator=(iir&&) = delete;
    constexpr explicit iir(
        const Vector<T, Allocator<T>>& a,
        const Vector<T, Allocator<T>>& b);
    constexpr explicit iir(
        T frequency,
        filter::type type,
        std::size_t order,
        characteristic characteristic);

    T filter(T sample);
    std::tuple<
        Vector<T, Allocator<T>>,
        Vector<T, Allocator<T>>> get_coefficients() const;
    constexpr void set_coefficients(
        const Vector<T, Allocator<T>>& a,
        const Vector<T, Allocator<T>>& b);
    constexpr void design(
        T frequency,
        filter::type type,
        std::size_t order,
        characteristic characteristic);

  private:
    struct data_t {
        Vector<T, Allocator<T>> a{};
        Vector<T, Allocator<T>> b{};
        Vector<T, Allocator<T>> xy{};
    };
    std::array<data_t, 2> m_datas{};
    data_t* m_data{&m_datas[0]};

    constexpr void m_butterworth(
        T frequency,
        filter::type type,
        std::size_t order);
};

/*******************************************************************************
 * @brief                   Construct from coefficients.
 * @param a                 The a-coefficients (denominator).
 * @param b                 The b-coefficients (numerator).
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr iir<T, Vector, Allocator>::iir(
        const Vector<T, Allocator<T>>& a,
        const Vector<T, Allocator<T>>& b) {
    set_coefficients(
        a,
        b);
}

/*******************************************************************************
 * @brief                   Desing filter with given characteristic.
 * @param frequency         The corner frequency of the filter.
 * @param type              The type of the filter (low-pass, high-pass,
 *                          band-pass, band-stop).
 * @param order             The desired order of the filter.
 * @param characteristic    The characteristic of the filter (Butterworth,
 *                          Chebyshev, Bessel, Elliptic, Linkwitz-Riley).
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr iir<T, Vector, Allocator>::iir(
        const T frequency,
        const filter::type type,
        const std::size_t order,
        const characteristic characteristic) {
    design(
        frequency,
        type,
        order,
        characteristic);
}

/*******************************************************************************
 * @param sample            The sample to process.
 * @return                  The processed sample.
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
T iir<T, Vector, Allocator>::filter(T sample) {
    T _value{};

    m_data->xy[0] = sample;

    for (std::size_t _i = (m_data->a.size() - 1); _i > 0; --_i) {
        _value += m_data->b[_i] * m_data->xy[_i];
        m_data->xy[0] -= m_data->a[_i] * m_data->xy[_i];
        m_data->xy[_i] = m_data->xy[_i - 1];
    }
    _value += m_data->b[0] * m_data->xy[0];

    return _value;
}

/*******************************************************************************
 * @return                  Tuple of the a- (denominator) and
 *                          b-coefficients (numerator).
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
std::tuple<
        Vector<T, Allocator<T>>,
        Vector<T, Allocator<T>>> iir<T, Vector, Allocator>::get_coefficients() const {
    return std::make_tuple(
        m_data->a,
        m_data->b);
}

/*******************************************************************************
 * @param a                 The a-coefficients (denominator).
 * @param b                 The b-coefficients (numerator).
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr void iir<T, Vector, Allocator>::set_coefficients(
        const Vector<T, Allocator<T>>& a,
        const Vector<T, Allocator<T>>& b) {
    if (a.size() != b.size()) {
        return;
    }

    auto data{&m_datas.at((m_data == &m_datas[0]) ? 1 : 0)};

    data->a = a;
    data->b = b;
    data->xy.resize(a.size());

    m_data = data;
}

/*******************************************************************************
 * @param frequency         The corner frequency of the filter.
 * @param type              The type of the filter (low-pass, high-pass,
 *                          band-pass, band-stop).
 * @param order             The desired order of the filter.
 * @param characteristic    The characteristic of the filter (Butterworth,
 *                          Chebyshev, Bessel, Elliptic, Linkwitz-Riley).
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr void iir<T, Vector, Allocator>::design(
        const T frequency,
        const filter::type type,
        const std::size_t order,
        const characteristic characteristic) {
    switch (characteristic) {
        case characteristic::butterworth:
            m_butterworth(
                frequency,
                type,
                order
            );
            break;
    }
}

/*******************************************************************************
 * @brief                   Design Butterworth-filter from prototype.
 ******************************************************************************/
template<
    typename T,
    template<class, class> typename Vector,
    template<class> typename Allocator>
constexpr void iir<T, Vector, Allocator>::m_butterworth(
        const T frequency,
        const filter::type type,
        const std::size_t order) {
    Vector<std::complex<double>, Allocator<std::complex<double>>> poles{};
    Vector<std::complex<double>, Allocator<std::complex<double>>> zeros{};

    double fs{1.0};                                    // sampling frequency
    double fc{1.0};                                    // cutoff frequency
    std::complex<double> gain{std::abs(std::pow(-fc, order))};
    // Pre-warp frequencies
    double f{2.0 / fs * std::tan(M_PI * static_cast<double>(frequency) / fs)};

    // Construct Butterworth prototype
    for (std::size_t _i = 0; _i < order; ++_i) {
        double angle{M_PI *
            (2.0 * static_cast<double>(_i) + static_cast<double>(order) + 1) /
            (2.0 * static_cast<double>(order))};
        poles.push_back(fc * std::exp(std::complex<double>{0, angle}));
    }

    // s-plane frequency transform
    std::tie(
            zeros,
            poles,
            gain) = sft<double, Vector, Allocator>(
        zeros,
        poles,
        gain,
        f,
        type);

    // Bilinear transform
    std::tie(
            zeros,
            poles,
            gain) = bilinear<double, Vector, Allocator>(
        zeros,
        poles,
        gain);

    // convert to transfer function
    auto data{&m_datas.at((m_data == &m_datas[0]) ? 1 : 0)};

    std::tie(
            data->a,
            data->b) = zp2h<T, double, Vector, Allocator>(
        zeros,
        poles,
        gain);
    data->xy.resize(order + 1);

    m_data = data;
}

}  // namespace toptica::tsp::iir
