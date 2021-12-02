/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2021
 *
 * @file        fit_quadratic.hpp
 * @brief       Class for fitting a quadratic polynomial.
 *
 * @author      Euteneuer, Arno <Arno.Euteneuer@toptica.com>
 *
 * @details
 *      A quadractic_fit object can be used to fit a quadratic polynomial to given xy data.
 *      It supports different types of containers like std::vector, std::array or TSP's static_vector.
 *      The underlying data-type must be of floating-point type (float, double) and the containers must
 *      support the at(pos) method.
 *
 *      A fit calculates 3 fit coefficients.
 *
 *      Usage example:
 *          using d_container = std::vector<double>;   // data container for x and y data
 *          using c_container = std::array<double, 3>; // container for the 3 coefficients
 *
 *          toptica::tsp::fit::fit_quadratic<d_container, c_container> qfit; // create fit object
 *
 *          const d_container x{-1, 0, 1}; // example x data, must have a size >= 3
 *          const d_container y{1, 0, 1};  // example y data, must have a size >= 3
 *          c_container       c;           // result container, must have a size >= 3
 *
 *          const auto ok = qfit(x, y, c); // do the fit
 *
 *          const auto c = c[0]; // offset coefficient
 *          const auto b = c[1]; // linear coefficient
 *          const auto a = c[2]; // quadratic coefficient
  *
 ******************************************************************************/

#pragma once

#include "fit_base.hpp"
#include <cmath>
#include <cstddef>

namespace toptica::tsp::fit {

/**
 * @brief Class for fitting a quadratic polynomial y = a*x^2 + b*x + c to data.
 * @tparam DataContainer Container type for x and y data.
 * @tparam ResultContainer Container type for coefficients. Must support at() method.
 * NOTE: The underlying data types of DataContainer and ResultContainer must be identical.
 */
template <typename DataContainer, typename ResultContainer>
class fit_quadratic : public fit_base<DataContainer, ResultContainer>
{
    using data_type = typename DataContainer::value_type;

  public:
    fit_quadratic();

    size_t      number_of_coeffs() const override;
    data_type   fit_value(data_type x) const override;

  protected:
    bool fit(const DataContainer& x,
             const DataContainer& y,
             size_t pos,
             size_t count,
             ResultContainer& coeffs) override;

  private:
    data_type a_{0.};
    data_type b_{0.};
    data_type c_{0.};
};

/**
 * @brief Ctor of the class which is calling the fit_base ctor.
 */
template <typename DataContainer, typename ResultContainer>
fit_quadratic<DataContainer, ResultContainer>::fit_quadratic()
  : fit_base<DataContainer, ResultContainer>()
{

}

/**
 * @brief Quadratic fits have 3 coefficients and need at least 3 data points.
 * @return 3
 */
template <typename DataContainer, typename ResultContainer>
size_t fit_quadratic<DataContainer, ResultContainer>::number_of_coeffs() const
{
    return 3;
}

/**
 * @brief Calculates the fit value a*x^2 + b*x + c at x-axis value x.
 * @param x X-value.
 * @return Value of the fit parabola.
 */
template <typename DataContainer, typename ResultContainer>
typename DataContainer::value_type fit_quadratic<DataContainer, ResultContainer>::fit_value(typename DataContainer::value_type x) const
{
    return a_ * x * x + b_ * x + c_;
}

 /**
 * @brief Function doing the actual quadratic fit to y = a*x^2 + b*x *c.
 * @param x Container with x-axis data.
 * @param y Container with y-axis data.
 * @param pos Index of the first sample point to use.
 * @param count Number of sample points to use.
 * @param coeffs Container to store the resulting coefficients in.
 * coeff[0] = c, coeff[1] = b, coeff[2] = a.
 * @return True if fit succeeded, false otherwise.
 */
template <typename DataContainer, typename ResultContainer>
bool fit_quadratic<DataContainer, ResultContainer>::fit(const DataContainer& x,
                                                        const DataContainer& y,
                                                        size_t pos,
                                                        size_t count,
                                                        ResultContainer& coeffs)
{
    const samples_t<DataContainer> samples(x, y, pos, count);

    const auto two = static_cast<data_type>(2);
    const auto s00 = static_cast<data_type>(count);
    data_type  s10{0};
    data_type  s20{0};
    data_type  s30{0};
    data_type  s40{0};
    data_type  s01{0};
    data_type  s11{0};
    data_type  s21{0};

    for (size_t i = samples.pos; i < samples.end; ++i) {
        const auto xi  = samples.x.at(i);
        const auto yi  = samples.y.at(i);
        const auto xi2 = xi * xi;
        s10 += xi;
        s20 += xi2;
        s30 += xi2 * xi;
        s40 += xi2 * xi2;
        s01 += yi;
        s11 += xi * yi;
        s21 += xi2 * yi;
    }

    const auto norm =
        (s00 * s20 * s40) - (s10 * s10 * s40) - (s00 * s30 * s30) + (two * s10 * s20 * s30) - (s20 * s20 * s20);

    a_ = ((s01 * s10 * s30) - (s11 * s00 * s30) - (s01 * s20 * s20) + (s11 * s10 * s20) + (s21 * s00 * s20) -
            (s21 * s10 * s10)) /
        norm;

    b_ = ((s11 * s00 * s40) - (s01 * s10 * s40) + (s01 * s20 * s30) - (s21 * s00 * s30) - (s11 * s20 * s20) +
            (s21 * s10 * s20)) /
        norm;

    c_ = ((s01 * s20 * s40) - (s11 * s10 * s40) - (s01 * s30 * s30) + (s11 * s20 * s30) + (s21 * s10 * s30) -
            (s21 * s20 * s20)) /
        norm;

    coeffs.at(0) = c_; // constant
    coeffs.at(1) = b_; // linear
    coeffs.at(2) = a_; // quadratic

    if (fit_base<DataContainer, ResultContainer>::diagnosis) {
        fit_base<DataContainer, ResultContainer>::calculate_diagnosis_(samples);
    }

    return true;
}

} // namespace toptica::tsp::fit
