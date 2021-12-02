/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2021
 *
 * @file        fit_ransac.hpp
 * @brief       Class for doing outlier tolerant fits.
 *              https://en.wikipedia.org/wiki/Random_sample_consensus
 *
 * @author      Euteneuer, Arno <Arno.Euteneuer@toptica.com>
 *
 * @details
 *      A fit_ransac object can be used to apply fits to a set of x and y samples, which has
 *      a certain amount of "outliers", such as errors in a measurement. The algorithm tries to
 *      identify "inliers" and "outliers" and applies the given fit function only to the inliers,
 *      ignoring the outliers.
 *
 *      The fit_ransac can be used with any fit function derived from the toptica::tsp::fit::fit_base class
 *      and containers of floating point data.
 *
 *      The fit function must be provided to the fit_ransac constructor.
 *      Two of the three template parameters are deduced from the fit function. The third parameter defaults to
 *      std::vector<size_t> and is necessary for internal calculations (random sampling).
 *      In environments where no heap is available, a different container type can be explicitly provided.
 *      For best performance (better than with default type) std::array<size_t, N> should be chosen.
 *
 *      The actual RANSAC fit is then done by calling the operator() of the fit_ransac object.
 *
 *      Usage example:
 *          using d_container = std::vector<double>;   // data container for x and y data
 *          using c_container = std::array<double, 3>; // container for the 3 coefficients
 *
 *          toptica::tsp::fit::fit_quadratic<d_container, c_container> qfit; // create fit object
 *
 *          toptica::tsp::fit::fit_ransac ransac(qfit); // create RANSAC fitter object
 *
 *          const d_container x{... 100 x-values ...}; // example x data
 *          const d_container y{... 100 y-values ...}; // example y data
 *          c_container       c;                       // result container, must have a size >= 3
 *
 *          // Now do the fit. We accept up to 40 outliers (100 - 60), which are defined by a deviation
 *          // of more than 0.5 from the a fit curve
 *          const auto ok = ransac(x, y, c, 60, 0.5);
 *
 *          // The result container now contains the coefficients for the best fit over inliers only.
 *          const auto c = c[0]; // offset coefficient
 *          const auto b = c[1]; // linear coefficient
 *          const auto a = c[2]; // quadratic coefficient
 *
 *
 ******************************************************************************/

#pragma once

#include "fit_base.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>

namespace {

/**
 * @brief Primary template of utility object for testing whether a container type provides a "reserve" method
 * for preallocating memory.
 * @tparam Container Container type to test.
 * @tparam Primary template handles types that have no reserve method.
 */
template <typename Container, typename = void>
struct has_reserve : std::false_type
{
};

/**
 * @brief Specialization template of utility object handles container types that do have reserve method.
 * @tparam Container Container type to test.
 */
template <typename Container>
struct has_reserve<
    Container,
    std::void_t<decltype(std::declval<Container>().reserve(std::declval<typename Container::size_type>()))>>
  : std::true_type
{
};

/**
 * @brief "optional_reserve" function does nothing for containers that have no reserve method.
 * @tparam Container Container type (Deduced from c).
 * @param c Container instance (unused).
 * @param n Size to reserve (unused).
 * @return void.
 */
template <typename Container>
std::enable_if_t<!has_reserve<Container>::value> optional_reserve(Container& c, size_t n)
{
    (void)c;
    (void)n;
}

/**
 * @brief "optional_reserve" calls reserve method if the container provides the method.
 * @tparam Container Container type (Deduced from c).
 * @param c Container instance to use for reserve.
 * @param n Size to reserve.
 * @return void.
 */
template <typename Container>
std::enable_if_t<has_reserve<Container>::value> optional_reserve(Container& c, size_t n)
{
    c.reserve(n);
}

/**
 * @brief Primary template of utility object for testing whether a container type provides a "push_back" method
 * for filling the container.
 * @tparam Container Container type to test.
 * @tparam Primary template handles types that have no push_back method.
 */
template <typename Container, typename = void>
struct has_push_back : std::false_type
{
};

/**
 * @brief Specialization template of utility object handles container types that do have push_back method.
 * @tparam Container Container type to test.
 */
template <typename Container>
struct has_push_back<
    Container,
    std::void_t<decltype(std::declval<Container>().push_back(std::declval<typename Container::value_type>()))>>
  : std::true_type
{
};

/**
 * @brief "optional_zero_init" function does nothing for containers that have no push_back method.
 * @tparam Container Container type (Deduced from c).
 * @param c Container instance (unused).
 * @param n Size to reserve (unused).
 * @return void.
 */
template <typename Container>
std::enable_if_t<!has_push_back<Container>::value> optional_zero_init(Container& c, size_t n)
{
    (void)c;
    (void)n;
}

/**
 * @brief "optional_zero_init" fills the container with zeros until it has the size n.
 * @tparam Container Container type (Deduced from c).
 * @param c Container instance to fill.
 * @param n Desired size of the container.
 * @return void.
 */
template <typename Container>
std::enable_if_t<has_push_back<Container>::value> optional_zero_init(Container& c, size_t n)
{
    while (c.size() < n) {
        c.push_back(static_cast<typename Container::value_type>(0.));
    }
}

/**
 * @brief Function for randomly selecting a certain number of indices between zero and a maximum value.
 * @tparam IndexContainer Container type to be used for the result. Underlying data_type must be size_t.
 * @param sample_count Number of indices to select.
 * @param max_value Maximum index value.
 * @return Container with sample_count randomly selected indices.
 */
template <typename IndexContainer>
IndexContainer get_random_samples(size_t sample_count, size_t max_value)
{
    static_assert(
        std::is_same<typename IndexContainer::value_type, size_t>::value,
        "IndexContainer must be a container of size_t elements");

    static std::default_random_engine     random_engine{};
    std::uniform_int_distribution<size_t> random_distribution(0, max_value);

    IndexContainer indices{};
    optional_reserve(indices, sample_count);   // for containers like std::vector reserves memory
    optional_zero_init(indices, sample_count); // for containers like std::vector and
                                               // std::static_vector assures correct size

    if (sample_count > (max_value + 1)) {
        return indices; // too few possible indices
    }

    size_t index_count = 0;
    auto   end         = indices.begin(); // iterator up to which we need to search for duplicates
    while (index_count < sample_count) {
        const auto new_index = random_distribution(random_engine);
        const auto existing  = std::find(indices.begin(), end, new_index);
        if (existing == end) {
            indices.at(index_count) = new_index;
            ++index_count;
            ++end; // next time we extend our search for duplicates to one additional element
        }
    }
    return indices;
}

} // namespace

namespace toptica::tsp::fit {

/**
 * @brief Class for doing RANSAC fits.
 *
 * @tparam DataContainer Container type for the x and y input data.
 * @tparam ResultContainer Containter type for the resulting fit coefficients. (Prefer std::array)
 * @tparam IndexContainer Containter type for indices (optional). Underlying type must be size_t. (Prefer std::array)
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer = std::vector<size_t>>
class fit_ransac
{
    using data_type = typename DataContainer::value_type;

  public:
    fit_ransac(fit_base<DataContainer, ResultContainer>& fit);

    bool operator()(
        const DataContainer& x,
        const DataContainer& y,
        ResultContainer&     best_coeffs,
        size_t               min_inlier_count,
        data_type            max_inlier_deviation,
        size_t               max_iteration_count = 0);

    size_t    number_of_inliers() const;
    data_type rmse() const;
    data_type r_square() const;
    data_type rss() const;

  private:
    fit_base<DataContainer, ResultContainer>& fit_; //!< Fit object.

    size_t    inliers_used_{0};   //!< Number of inliers used.
    data_type min_rss_{-1.};      //!< residual-sum-of-squares best final fit
    data_type max_r_square_{-1.}; //!< R-square of best fit
    data_type min_rmse_{-1.};     //!< root-mean-square-error of the best fit
};

/**
 * @brief Constructor of the RANSAC object.
 * @param fit A fit object, derived from the toptica::tsp::fit::fit_base class.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
fit_ransac<DataContainer, ResultContainer, IndexContainer>::fit_ransac(fit_base<DataContainer, ResultContainer>& fit)
  : fit_{fit}
{
}

/**
 * @brief Apply the outlier tolerant fit to the given input data.
 * @param x Container with x-axis data.
 * @param y Container with y-axis data. x and y must be of same type and must have the same size.
 * @param best_coeffs Container to store the resulting coefficients.
 * @param min_inlier_count Minimum number of inliers required for a successful fit.
 *        This must be chosen based on the expected number of outliers and should be >= 50% of the
 *        total number of samples.
 * @param max_inlier_deviation Criterion for discriminating between inliers and outliers.
 *        Samples with a y value differing more than max_inlier_deviation from the fit value at
 *        the respective x position are considered outliers. All other samples are inliers.
 * @param max_iteration_count (optional) Number of iterations to perform.
 *        If no number is given, the number of iterations is calculated from min_inlier_count
 *        to achieve a 99% probability of finding a fit candidate with enough inliers.
 * @return True if the fit was successful and false if not.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
bool fit_ransac<DataContainer, ResultContainer, IndexContainer>::operator()(
    const DataContainer& x,
    const DataContainer& y,
    ResultContainer&     best_coeffs,
    size_t               min_inlier_count,
    data_type            max_inlier_deviation,
    size_t               max_iteration_count)
{
    if (x.size() < fit_.number_of_coeffs()) {
        return false;
    }
    if (x.size() != y.size()) {
        return false;
    }
    if (best_coeffs.size() < fit_.number_of_coeffs()) {
        return false;
    }
    if (min_inlier_count < fit_.number_of_coeffs()) {
        return false;
    }

    /* From the probability for finding an inlier in the data, we calculate the number of iterations
     * necessary to achieve a 99% probability for successful RANSAC fit. */
    const auto probability = static_cast<data_type>(min_inlier_count) / static_cast<data_type>(x.size());
    const auto trials99    = std::log(1. - 0.99) / std::log(1. - std::pow(probability, fit_.number_of_coeffs()));

    const size_t iteration_count = std::max<size_t>(
        (max_iteration_count == 0) ? static_cast<size_t>(std::round(trials99)) : max_iteration_count, 1);

    /* Containers for collecting inliers must be of same type and size as data */
    DataContainer consensus_x(x);
    DataContainer consensus_y(y);

    /* Container for intermediate coefficient results must be of same type and size as the
     * container best_coeffs for the final coefficients. However, copy-constructor from non-const containers
     * doesn't work for static_vector, for example. */
    ResultContainer test_coeffs{};
    optional_reserve(test_coeffs, fit_.number_of_coeffs());   // for containers like std::vector reserves memory
    optional_zero_init(test_coeffs, fit_.number_of_coeffs()); // for containers like std::vector and static_vector
                                                              // assures correct size

    /* We need to find the lowest root-mean-square-error, so we initialize with highest number. */
    min_rmse_ = std::numeric_limits<data_type>::max();

    for (size_t i = 0; i < iteration_count; ++i) {

        /* According to the number of coefficients/degrees of freedom  we choose randomly selected
         * samples from the input data. */
        const auto samples = get_random_samples<IndexContainer>(fit_.number_of_coeffs(), x.size() - 1);
        for (size_t k = 0; k < fit_.number_of_coeffs(); ++k) {
            consensus_x.at(k) = x.at(samples.at(k));
            consensus_y.at(k) = y.at(samples.at(k));
        }

        /* We apply a fit to the random samples ... */
        fit_.diagnosis = false; // no diagnosis necessary here
        if (!fit_(consensus_x, consensus_y, 0, fit_.number_of_coeffs(), test_coeffs)) {
            continue;
        }

        /* ... and collect all "inliers", i.e. all other samples, which lie closer than
         * max_inlier_deviation to the fit. */
        size_t ninlier = 0;
        for (size_t k = 0; k < x.size(); ++k) {
            const data_type d = std::abs(y.at(k) - fit_.fit_value(x.at(k)));
            if (d <= std::abs(max_inlier_deviation)) {
                consensus_x.at(ninlier) = x.at(k);
                consensus_y.at(ninlier) = y.at(k);
                ninlier++;
            }
        }

        /* If sufficient inliers can be found, this might be a candidate for the final fit. */
        if (ninlier >= min_inlier_count) {

            /* Now we apply the fit to all inliers ... */
            fit_.diagnosis = true; // necessary in order to calculate rmse.
            if (!fit_(consensus_x, consensus_y, 0, ninlier, test_coeffs)) {
                continue; // least-square fitting fails;
            }

            /* ... and check whether the result was better than during preview iterations.*/
            if (fit_.rmse() < min_rmse_) {
                best_coeffs   = test_coeffs;
                min_rmse_     = fit_.rmse();
                min_rss_      = fit_.rss();
                max_r_square_ = fit_.r_square();
                inliers_used_ = ninlier;
            }

            /* If there are no outliers at all, we don't need to continue iterating. */
            if (ninlier == x.size()) {
                break;
            }
        }
    }
    return min_rmse_ < std::numeric_limits<data_type>::max();
}

/**
 * @brief Get the number of inliers used for the final fit result.
 * @return Number of inliers.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
size_t fit_ransac<DataContainer, ResultContainer, IndexContainer>::number_of_inliers() const
{
    return inliers_used_;
}

/**
 * @brief Get the root-mean-square-error of the final fit.
 * @return Root-mean-square-error of the inlier subset of the data.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
typename DataContainer::value_type fit_ransac<DataContainer, ResultContainer, IndexContainer>::rmse() const
{
    return min_rmse_;
}

/**
 * @brief Get the r-square value of the final fit.
 * @return R-square value of the inlier subset of the data.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
typename DataContainer::value_type fit_ransac<DataContainer, ResultContainer, IndexContainer>::r_square() const
{
    return max_r_square_;
}

/**
 * @brief Get the residual-sum-of-squares of the final fit.
 * @return Residual-sum-of-squares of the inlier subset of the data.
 */
template <typename DataContainer, typename ResultContainer, typename IndexContainer>
typename DataContainer::value_type fit_ransac<DataContainer, ResultContainer, IndexContainer>::rss() const
{
    return min_rss_;
}

} // namespace toptica::tsp::fit
