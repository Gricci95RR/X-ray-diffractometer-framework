/* © Copyright CERN 2021.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Santiago Andres Solis Paiva CERN BE/CEM/MRO 2021
 *         
 *
 *  ===============================================================================================
 */

#include <gtest/gtest.h>
#include "../include/Template/Hello.hpp"  // Where is the header with respect to the tests

bool f() {  // helper function in the test
    return true;
}

struct ExampleTests
    : public ::testing::Test{
    int* x;
    int GetX() {
        return *x;
    }

    void SetUp() override {
        // Will only be seen in the terminal
        fprintf(stderr, "Hello, Test!\n");
        x = new int(42);
    }

    void TearDown() override {
        // Will only be seen in the terminal
        fprintf(stderr, "Tearing down!\n");
        delete x;
    }
};

// This macro _F adds the fixture component into the test
TEST_F(ExampleTests, DemonstrateGTestMacros) {
    EXPECT_TRUE(true);
    ASSERT_FALSE(false);  // Code doesnt pass from there if it fails
    EXPECT_TRUE(true);
    EXPECT_EQ(true, true);
    const bool result = f();
    EXPECT_EQ(true, result)
        << "Hello,World!";  // Text will appear if the test FAILS
}

TEST_F(ExampleTests, MAC) {
    int x = 42;
    int y = 16;
    int sum = 100;
    int oldSum = sum;
    int expectedNewSum = oldSum + x*y;

    EXPECT_EQ(
        expectedNewSum,
        expectedNewSum);  // MAC(x,y,sum)
    EXPECT_EQ(
        expectedNewSum,
        expectedNewSum) << "This is expected to fail";
}

TEST_F(ExampleTests, Square) {
    int x = 5;
    int expectedSquare = x*x;
    EXPECT_EQ(
        expectedSquare,
        expectedSquare);  // Square(x)
}

TEST_F(ExampleTests, Square_Fixture) {
    int expectedSquare = GetX()*GetX();
    EXPECT_EQ(
        expectedSquare,
        expectedSquare);  // Square(GetX())
}
