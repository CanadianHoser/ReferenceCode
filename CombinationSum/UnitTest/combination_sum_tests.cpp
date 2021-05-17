//
//  combination_sum_tests.cpp
//
//

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include <string.h>

extern "C" {
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

// These are used to validate my understanding of the isalpha() family of functions.
// They would be removed if this were production code, but retained to show how I use
// the UT infra to confirm my understanding of unfamiliar functios.
TEST_GROUP(combination_sum)
{
    void setup() { }
    void teardown() { }
};

TEST(combination_sum, initial_test)
{
	CHECK_TRUE(2 == 2);
}
