#include "gtest/gtest.h"
#include <algorithm>
#include <array>
#include "../include/spscrb_adapter.h"

TEST(TestAdapter, TestPushbackSimple) {
    spscrb::spscrb_adapter rb{};

    ASSERT_EQ(rb.size(), 0);
    
    rb.push_back(1);
    ASSERT_EQ(rb.size(), 1);
    
    rb.push_back(2);
    ASSERT_EQ(rb.size(), 2);
    
    rb.push_back(3);
    ASSERT_EQ(rb.size(), 3);
    
    rb.push_back(4);
    ASSERT_EQ(rb.size(), 4);


    ASSERT_EQ(rb.top(), 1);
    rb.pop_front();
    ASSERT_EQ(rb.size(), 3);

    rb.push_back(5);
    ASSERT_EQ(rb.size(), 4);
    ASSERT_EQ(rb.top(), 2);

    ASSERT_EQ(rb.at(0), 2);
    ASSERT_EQ(rb.at(1), 3);
    ASSERT_EQ(rb.at(2), 4);
    ASSERT_EQ(rb.at(3), 5);


    rb.at(1) += 10;
    ASSERT_EQ(rb.at(0), 2);
    ASSERT_EQ(rb.at(1), 13);
    ASSERT_EQ(rb.at(2), 4);
    ASSERT_EQ(rb.at(3), 5);

    for (auto&& el : rb)
        std::cout << static_cast<int>(el) << " ";
    std::cout << std::endl;

    std::sort(rb.begin(), rb.end(), std::greater<int>());
    for (auto&& el : rb)
        std::cout << static_cast<int>(el) << " ";
    std::cout << std::endl;

    ASSERT_TRUE(std::equal(rb.begin(), rb.end(), std::array<char, 4>({ 13, 5, 4, 2 }).begin()));
    ASSERT_FALSE(std::equal(rb.begin(), rb.end(), std::array<char, 4>({ 15, 5, 4, 2 }).begin()));
}
