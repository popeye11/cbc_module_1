/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        static_vector.hpp
 * @brief       A static (fixed capacity) vector.
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

#include <container/container.hpp>

#include <algorithm>
#include <array>
#include <utility>


namespace toptica::container {

template<typename T, std::size_t Size>
class static_vector : public std::array<T, Size> {
  public:
    using size_type = typename std::array<T, Size>::size_type;
    using iterator = typename std::array<T, Size>::iterator;
    using const_iterator = typename std::array<T, Size>::const_iterator;
    using reference = typename std::array<T, Size>::reference;
    using const_reference = typename std::array<T, Size>::const_reference;

    template<typename ...U>
    explicit static_vector(U&&... array);

    constexpr size_type         size() const noexcept;
    constexpr size_type         capacity() const noexcept;
    constexpr bool              empty() const noexcept;
    iterator                    begin() noexcept;
    const_iterator              begin() const noexcept;
    const_iterator              cbegin() const noexcept;
    iterator                    end() noexcept;
    const_iterator              end() const noexcept;
    const_iterator              cend() const noexcept;
    iterator                    erase(iterator position);
    iterator                    erase(iterator first, iterator last);
    reference                   at(size_type n);
    constexpr const_reference   at(size_type __n) const;
    error_code                  push_back(const T& value);
    error_code                  push_back(T&& value);

  private:
    size_type m_size{0};
};

template<typename T, std::size_t Size>
template<typename ...U>
static_vector<T, Size>::static_vector(U&&... array)
  : std::array<T, Size>{{std::forward<U>(array)...}} {
    m_size = sizeof...(array);
}

template<typename T, std::size_t Size>
constexpr typename std::array<T, Size>::size_type static_vector<T, Size>::size() const noexcept
{
    return m_size;
}

template<typename T, std::size_t Size>
constexpr typename std::array<T, Size>::size_type static_vector<T, Size>::capacity() const noexcept
{
    return Size;
}

template<typename T, std::size_t Size>
constexpr bool static_vector<T, Size>::empty() const noexcept
{
    return m_size == 0;
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::iterator static_vector<T, Size>::begin() noexcept
{
    return iterator(std::array<T, Size>::data());
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::const_iterator static_vector<T, Size>::begin() const noexcept
{
    return const_iterator(std::array<T, Size>::data());
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::const_iterator static_vector<T, Size>::cbegin() const noexcept
{
    return const_iterator(std::array<T, Size>::data());
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::iterator static_vector<T, Size>::end() noexcept
{
    return iterator(std::array<T, Size>::data() + m_size);
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::const_iterator static_vector<T, Size>::end() const noexcept
{
    return const_iterator(std::array<T, Size>::data() + m_size);
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::const_iterator static_vector<T, Size>::cend() const noexcept
{
    return const_iterator(std::array<T, Size>::data() + m_size);
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::iterator static_vector<T, Size>::erase(iterator position)
{
    if (position + 1 != end()) {
        std::copy(position + 1, end(), position);
        --m_size;
    }
    return position;
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::iterator static_vector<T, Size>::erase(iterator first, iterator last)
{
    if (first == begin() && last == end()) {
        m_size = 0;
        return end();
    }
    if (first != last) {
        std::copy(last, end(), first);
        m_size -= static_cast<size_type>(last - first);
    }
    return first;
}

template<typename T, std::size_t Size>
typename std::array<T, Size>::reference static_vector<T, Size>::at(size_type n)
{
    if (n >= m_size) {
        std::__throw_out_of_range_fmt(__N("static_vector::at: n (which is %zu) "
                ">= m_size (which is %zu)"),
            n, m_size);
    }
    return std::array<T, Size>::at(n);
}

template<typename T, std::size_t Size>
constexpr typename std::array<T, Size>::const_reference static_vector<T, Size>::at(size_type __n) const
{
    // Result of conditional expression must be an lvalue so use
    // boolean ? lvalue : (throw-expr, lvalue)
    return __n < m_size ? std::array<T, Size>::at(__n) :
        (std::__throw_out_of_range_fmt(__N("static_vector::at: __n (which is %zu) "
                ">= m_size (which is %zu)"),
            __n, m_size),
        std::array<T, Size>::at(0));
}

template<typename T, std::size_t Size>
error_code static_vector<T, Size>::push_back(const T& value)
{
    if (m_size >= Size) {
        return error_code::container_full;
    }
    (*this)[m_size] = value;
    m_size++;
    return error_code::ok;
}

template<typename T, std::size_t Size>
error_code static_vector<T, Size>::push_back(T&& value)
{
    if (m_size >= Size) {
        return error_code::container_full;
    }
    (*this)[m_size] = std::forward<T>(value);
    m_size++;
    return error_code::ok;
}

}  // namespace toptica::container
