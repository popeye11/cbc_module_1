/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2021
 *
 * @file        fit_base.hpp
 * @brief       A base class for fit algorithms.
 *
 * @author      Euteneuer, Arno <Arno.Euteneuer@toptica.com>
 *
 * @details
 *      The fit fit_base class is intended to provide a standardized API for fit functions.
 *      It supports different types of containers like std::vector, std::array or TSP's static_vector.
 *      The underlying data-type must be of floating-point type (float, double) and the containers must
 *      support the at(pos)  method.
 *
 *      Derived classes need to implement the following methods:
 *
 *      1. virtual bool fit(const DataContainer& x, const DataContainer& y, size_t pos, size_t n, ResultContainer& c)
 *      2. virtual size_t number_of_coeffs() const
 *      3. virtual data_type fit_value(data_type x) const
 *
 *      1) The fit method must calculate the fit coefficients for the xy date provided in x and y and store
 *         the result in the c container.
 *         If the diagnosis flag is true, the method must also calculate the error values rss_, rmse_ and r_square_.
 *         This calculation can be done with the protected calculate_diagnosis_ method of the fit_base class or
 *         directly during the fitting algorithm.
 *         The fit function will not be called directly by the application and can assume that
 *         all necessary range checks have been done before it is called.
 *
 *      2) The number_of_coeffs method must return the number of coefficients the fit function contains.
 *
 *      3) After a fit has been performed, the fit_value method must calculate the y-value of the fit function
 *         at the given x-axis position.
 *
 *      The application must use one of the operator() to perform the fit.
 *
 *
 ******************************************************************************/

#pragma once
#include <cmath>
#include <cstddef>
#include <type_traits>

namespace toptica::tsp::fit {

/**
 * @brief Struct containing the sample containers and the index range to be fitted.
 * @tparam Container Container type for x and y data.
 */
template <typename Container>
struct samples_t
{
    samples_t(const Container& the_x, const Container& the_y, size_t the_pos, size_t the_n)
      : x{the_x}, y{the_y}, pos{the_pos}, n{the_n}, end{the_pos + the_n} {};

    const Container& x;   //!< Container with x-axis data.
    const Container& y;   //!< Container with y-axis data.
    const size_t     pos; //!< First index position to used from x and y containers.
    const size_t     n;   //!< Number of samples to use, starting from pos.
    const size_t     end; //!< One index past the last sample to use.
};

/**
 * @brief Base class for fit algorithms.
 *
 * @tparam DataContainer Container type for x and y data.
 * @tparam ResultContainer Container type for coefficients.
 * NOTE: The underlying data types of DataContainer and ResultContainer must be identical.
 */
template <typename DataContainer, typename ResultContainer>
class fit_base
{
    using data_type = typename DataContainer::value_type;

  public:
    fit_base();
    virtual ~fit_base() = default;

    fit_base(const fit_base&) = default;
    fit_base& operator=(const fit_base&) = default;
    fit_base(fit_base&&) noexcept        = default;
    fit_base& operator=(fit_base&&) noexcept = default;

    virtual size_t      number_of_coeffs() const = 0;
    bool                operator()(const DataContainer& x, const DataContainer& y, ResultContainer& c);
    bool                operator()(const DataContainer& x, const DataContainer& y, size_t pos, size_t n, ResultContainer& c);
    bool                operator()(const samples_t<DataContainer>& samples, ResultContainer& c);
    virtual data_type   fit_value(data_type x) const = 0;

    /**
     * Flag to specify whether diagnosis data should be generated along with the fitting procedure.
     */
    bool diagnosis{false};

    data_type rss() const;
    data_type r_square() const;
    data_type rmse() const;

  protected:
    virtual bool fit(const DataContainer& x, const DataContainer& y, size_t pos, size_t n, ResultContainer& c) = 0;
    void         calculate_diagnosis_(const samples_t<DataContainer>& samples);

    data_type rss_{-1.};      /// to be calculate in fit function if diagnosis flag is set
    data_type r_square_{-1.}; /// to be calculate in fit function if diagnosis flag is set
    data_type rmse_{-1.};     /// to be calculate in fit function if diagnosis flag is set


};

/**
 * @brief Ctor of the class. Performs data consistency checks based on type.
 */
template <typename DataContainer, typename ResultContainer>
fit_base<DataContainer, ResultContainer>::fit_base()
{
    static_assert(
        std::is_floating_point<typename DataContainer::value_type>::value, "only floating data types supported");

    static_assert(
        std::is_same<typename DataContainer::value_type, typename ResultContainer::value_type>::value,
        "type of coefficients must be the same as type of xy data");
}

/**
 * @brief Returns the number of fit coefficients, which at the same time is the minimum
 * number of necessary data points and required size for the ResultContainer.
 * @return Algorithm-specific number of fit coefficients.
 */
template <typename DataContainer, typename ResultContainer>
size_t fit_base<DataContainer, ResultContainer>::number_of_coeffs() const
{
    return 0;
}

/**
 * @brief Applies the fit algorithm to the given x and y data and returns the resulting
 * coefficients in c.
 * @param x Container with x-axis data.
 * @param y Container with y-axis data. y and c must be of same type and must have the same size.
 * @param c Container to store the resulting coefficients.
 * @return True if fit succeded, false otherwise.
 */
template <typename DataContainer, typename ResultContainer>
bool fit_base<DataContainer, ResultContainer>::operator()(const DataContainer& x,
                                                      const DataContainer& y,
                                                      ResultContainer& c)
{
    if (x.size() < number_of_coeffs()) {
        return false;
    }
    if (x.size() != y.size()) {
        return false;
    }
    if (c.size() < number_of_coeffs()) {
        return false;
    }
    rss_      = -1.;
    rmse_     = -1.;
    r_square_ = -1.;
    return fit(x, y, 0, x.size(), c);
}

/**
 * @brief Applies the fit algorithm to a subset of the given x and y data and returns the resulting
 * coefficients in c.
 * @param x Container with x-axis data.
 * @param y Container with y-axis data. y and c must be of same type.
 * @param pos Index of the first sample point to use.
 * @param n Number of sample points to use.
 * @param c Container to store the resulting coefficients in.
 * @return True if fit succeeded, false otherwise.
 */
template <typename DataContainer, typename ResultContainer>
bool fit_base<DataContainer, ResultContainer>::operator()(const DataContainer& x,
                                                      const DataContainer& y,
                                                      size_t pos,
                                                      size_t n,
                                                      ResultContainer& c)
{
    if (n < number_of_coeffs()) {
        return false;
    }
    const size_t min_size = pos + n;
    if (x.size() < min_size || y.size() < min_size) {
        return false;
    }
    if (c.size() < number_of_coeffs()) {
        return false;
    }
    rss_      = -1.;
    rmse_     = -1.;
    r_square_ = -1.;
    return fit(x, y, pos, n, c);
}

/**
 * @brief Applies the fit algorithm to data given in the samples struct and returns the resulting
 * coefficients in c.
 * @param samples Sample data to be fitted.
 * @param c Container to store the resulting coefficients in.
 * @return True if fit succeeded, false otherwise.
 */
template <typename DataContainer, typename ResultContainer>
bool fit_base<DataContainer, ResultContainer>::operator()(const samples_t<DataContainer>& samples,
                                                      ResultContainer& c)
{
    return operator()(samples.x, samples.y, samples.pos, samples.n, c);
}

/**
 * @brief Returns the fit value at x-axis position x.
 * Needs to be overridden in every derived class.
 * @param x X-axis value.
 * @return Y-axis value of the fit curve.
 */
template <typename DataContainer, typename ResultContainer>
typename DataContainer::value_type fit_base<DataContainer, ResultContainer>::fit_value(typename DataContainer::value_type x) const
{
    (void)x;    // Supress compiler warnings
    return 0;
}

/**
 * @brief If diagnosis flag was set before doing a fit this function returns the
 * "residual sum of squares" or "sum of squared estimate of errors".
 * If the diagnosis flag was not set, the return value -1.
 * @return Positive value. The smaller the value the better the fit result.
 */
template <typename DataContainer, typename ResultContainer>
typename DataContainer::value_type fit_base<DataContainer, ResultContainer>::rss() const
{
    return rss_;
}

/**
 * @brief If diagnosis flag was set before doing a fit this function returns the
 * "R-square" value of the fit.
 * If the diagnosis flag was not set, the return value is -1.
 * @return Value between 0 and 1. The closer to one, the better the result.
 */
template <typename DataContainer, typename ResultContainer>
typename DataContainer::value_type fit_base<DataContainer, ResultContainer>::r_square() const
{
    return r_square_;
}

/**
 * @brief If diagnosis flag was set before doing a fit this function returns the
 * "root mean square error" value of the fit.
 * If the diagnosis flag was not set, the return value is -1.
 * @return Positive value. The smaller the value the better the fit result.
 */
template <typename DataContainer, typename ResultContainer>
typename DataContainer::value_type fit_base<DataContainer, ResultContainer>::rmse() const
{
    return rmse_;
}

/**
 * @brief Actual fitting algorithm to be implemented in derived classes.
 * The fit function must operate on the subset n sample point of the x and y data starting from index pos.
 * Range checks are done in the function call operators of this base class.
 * @param x Container with x-axis data.
 * @param y Container with y-axis data.
 * @param pos Index of the first sample point to use.
 * @param n Number of sample points to use.
 * @param c Container to store the resulting coefficients in.
 * @return True if fit succeeded, false otherwise.
 */
template <typename DataContainer, typename ResultContainer>
bool fit_base<DataContainer, ResultContainer>::fit(const DataContainer& x,
                                               const DataContainer& y,
                                               size_t pos,
                                               size_t n,
                                               ResultContainer& c)
{
    (void)x;    // Supress compiler warnings
    (void)y;    // Supress compiler warnings
    (void)pos;  // Supress compiler warnings
    (void)n;    // Supress compiler warnings
    (void)c;    // Supress compiler warnings
    return false;
}

 /**
 * @brief Helper function to calculate the diagnosis values.
 * This method can be called in the fit() method of a derived class if the diagnosis flag is set.
 * Alternatively the derived class can calculate the diagnosis values during the fitting procedure
 * if this has a better performance.
 * @param samples The data provided to the latest fit.
 */
template <typename DataContainer, typename ResultContainer>
void fit_base<DataContainer, ResultContainer>::calculate_diagnosis_(const samples_t<DataContainer>& samples)
{
    data_type sum_y                = 0.0;
    data_type sum_residual_squared = 0.0;
    for (size_t i = samples.pos; i < samples.end; ++i) {
        const auto yi       = samples.y.at(i);
        const auto xi       = samples.x.at(i);
        const auto residual = yi - fit_value(xi);
        sum_y += yi;
        sum_residual_squared += (residual * residual);
    }
    const auto y_mean                = sum_y / static_cast<data_type>(samples.n);
    data_type  sum_deviation_squared = 0.0;
    for (size_t i = samples.pos; i < samples.end; ++i) {
        const auto yi        = samples.y.at(i);
        const auto deviation = yi - y_mean;
        sum_deviation_squared += (deviation * deviation);
    }

    rss_      = sum_residual_squared;
    r_square_ = static_cast<data_type>(1) - (sum_residual_squared / sum_deviation_squared);

    const auto denominator = samples.n - number_of_coeffs();
    if (denominator == 0) {
        rmse_ = static_cast<data_type>(0);
    } else {
        rmse_ = std::sqrt(sum_residual_squared / static_cast<data_type>(denominator));
    }
}

} // namespace toptica::tsp::fit
