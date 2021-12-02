/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        test_static_vector.cpp
 * @brief       Unit Tests for the static (fixed content size) vector.
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
#include <container/static_vector.hpp>
#include <cstdint>

using namespace toptica::container;

constexpr std::size_t VECTOR_SIZE{2};

BOOST_AUTO_TEST_SUITE(static_vector)

    BOOST_AUTO_TEST_CASE(instantiation_empty) {
        BOOST_TEST_MESSAGE("static_vector: correct instantiation of an empty vector");

        // Instantiate the vector
        toptica::container::static_vector<float, VECTOR_SIZE> vector{};
        BOOST_TEST_CHECK(vector.max_size() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.capacity() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.size() == 0);
        BOOST_TEST_CHECK(vector.empty() == true);
    }

    BOOST_AUTO_TEST_CASE(instantiation_one) {
        BOOST_TEST_MESSAGE("static_vector: correct instantiation of the vector with one element");

        // Instantiate the vector
        toptica::container::static_vector<float, VECTOR_SIZE> vector{2.2F};
        BOOST_TEST_CHECK(vector.max_size() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.capacity() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.size() == 1);
        BOOST_TEST_CHECK(vector.empty() == false);
        BOOST_TEST_CHECK(vector[0] == 2.2F);
        BOOST_TEST_CHECK(*vector.begin() == 2.2F);
        BOOST_TEST_CHECK(vector.begin() == &vector[0]);
        BOOST_TEST_CHECK(vector.end() == &vector[1]);
    }

    BOOST_AUTO_TEST_CASE(instantiation_two) {
        BOOST_TEST_MESSAGE("static_vector: correct instantiation of the vector with two elements");

        // Instantiate the vector
        toptica::container::static_vector<float, VECTOR_SIZE> vector{3.3F, 4.4F};
        BOOST_TEST_CHECK(vector.max_size() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.capacity() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.size() == 2);
        BOOST_TEST_CHECK(vector.empty() == false);
        BOOST_TEST_CHECK(vector[0] == 3.3F);
        BOOST_TEST_CHECK(vector[1] == 4.4F);
        BOOST_TEST_CHECK(*vector.begin() == 3.3F);
        BOOST_TEST_CHECK(vector.begin() == &vector[0]);
        BOOST_TEST_CHECK(vector.end() == &vector[VECTOR_SIZE]);
    }

    BOOST_AUTO_TEST_CASE(
            push_back,
            * boost::unit_test::depends_on("static_vector/instantiation_empty")) {
        BOOST_TEST_MESSAGE("static_vector: push_back of element");

        // Instantiate the vector
        toptica::container::static_vector<float, VECTOR_SIZE> vector{};
        for (std::size_t i = 0; i < vector.max_size(); i++) {
            error_code e = vector.push_back(5.5F + static_cast<float>(i) * 1.1F);
            BOOST_TEST_CHECK(vector.max_size() == VECTOR_SIZE);
            BOOST_TEST_CHECK(vector.capacity() == VECTOR_SIZE);
            BOOST_TEST_CHECK(vector.size() == i + 1);
            BOOST_TEST_CHECK(vector.empty() == false);
            BOOST_TEST_CHECK(vector[i] == 5.5F + static_cast<float>(i) * 1.1F);
            BOOST_TEST_CHECK(*vector.begin() == 5.5F);
            BOOST_TEST_CHECK(vector.begin() == &vector[0]);
            BOOST_TEST_CHECK(vector.end() == &vector[1 + i]);
            BOOST_TEST_CHECK((e == error_code::ok));
        }
        error_code e = vector.push_back(666.666F);
        BOOST_TEST_CHECK(vector.max_size() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.capacity() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.size() == VECTOR_SIZE);
        BOOST_TEST_CHECK(vector.empty() == false);
        BOOST_TEST_CHECK(*vector.begin() == 5.5F);
        BOOST_TEST_CHECK(vector.begin() == &vector[0]);
        BOOST_TEST_CHECK(vector.end() == &vector[VECTOR_SIZE]);
        BOOST_TEST_CHECK((e == error_code::container_full));
    }

    BOOST_AUTO_TEST_CASE(
            erase,
            * boost::unit_test::depends_on("static_vector/instantiation_empty")) {
        BOOST_TEST_MESSAGE("static_vector: erase");

        // Instantiate the vector
        toptica::container::static_vector<int, 10> vector{
            0,
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9
        };

        vector.erase(vector.begin());

        BOOST_TEST_CHECK(vector[0] == 1);
        BOOST_TEST_CHECK(vector[1] == 2);
        BOOST_TEST_CHECK(vector[2] == 3);
        BOOST_TEST_CHECK(vector[3] == 4);
        BOOST_TEST_CHECK(vector[4] == 5);
        BOOST_TEST_CHECK(vector[5] == 6);
        BOOST_TEST_CHECK(vector[6] == 7);
        BOOST_TEST_CHECK(vector[7] == 8);
        BOOST_TEST_CHECK(vector[8] == 9);
        BOOST_TEST_CHECK(vector.size() == 9);

        vector.erase(vector.begin() + 2, vector.begin() + 5);

        BOOST_TEST_CHECK(vector[0] == 1);
        BOOST_TEST_CHECK(vector[1] == 2);
        BOOST_TEST_CHECK(vector[2] == 6);
        BOOST_TEST_CHECK(vector[3] == 7);
        BOOST_TEST_CHECK(vector[4] == 8);
        BOOST_TEST_CHECK(vector[5] == 9);
        BOOST_TEST_CHECK(vector.size() == 6);

        // Erase all even numbers
        vector.erase(std::remove_if(vector.begin(), vector.end(), [](auto x){ return (x % 2 == 0); }), vector.end());

        BOOST_TEST_CHECK(vector[0] == 1);
        BOOST_TEST_CHECK(vector[1] == 7);
        BOOST_TEST_CHECK(vector[2] == 9);
        BOOST_TEST_CHECK(vector.size() == 3);
    }

    BOOST_AUTO_TEST_CASE(
            erase_all,
            *boost::unit_test::depends_on("static_vector/instantiation_empty")) {
        BOOST_TEST_MESSAGE("static_vector: erase_all");

        // Instantiate the vector
        toptica::container::static_vector<int, 10> vector{0, 1, 2, 3, 4,
                                                            5, 6, 7, 8, 9};

        vector.erase(vector.begin(), vector.end());

        BOOST_TEST_CHECK(vector.size() == 0);
    }

BOOST_AUTO_TEST_SUITE_END()
