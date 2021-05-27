//
//  linked_lists_tests.cpp
//  HelloWorld
//
//  Created by Warren Hein on 3/10/21.
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"

extern "C" {
    extern bool bracesComplete(const char *string);
	extern bool isCandidateOpen(char c);
	extern bool isCandidateClose(char matchTo, char candidate);
	extern const char *findOpenCandidate(const char *string);
}

TEST_GROUP(match_braces)
{
    void setup() override
    {
    }

    void teardown() override
    {
    }
};

TEST(match_braces, testHarness)
{
	LONGS_EQUAL(2,2);
}

TEST(match_braces, findOpenCandidate)
{
	const char *test="{jfkd";
	CHECK_TRUE(isCandidateOpen(*test));
}

TEST(match_braces, cannotFindOpenCandidate)
{
	const char *test="jfkd";
	CHECK_FALSE(isCandidateOpen(*test));
}

TEST(match_braces, cannotFindNextOpenCandidate)
{
	const char *test = nullptr;
    STRCMP_EQUAL(nullptr, findOpenCandidate(test));
}

TEST(match_braces, cannotFindNextOpenCandidateForEmptyString)
{
	const char *test = "";
    STRCMP_EQUAL(nullptr, findOpenCandidate(test));
}

TEST(match_braces, canFindNextOpenCandidateAtStartOfString)
{
	const char *test="{jfkd";
    STRCMP_EQUAL(test, findOpenCandidate(test));
}

TEST(match_braces, canFindNextOpenCandidateInMiddleOfString)
{
	const char *test="jf{kd";
    STRCMP_EQUAL("{kd", findOpenCandidate(test));
}

TEST(match_braces, shouldNotFindOpenCandidateAtEndOfString)
{
	const char *test="jfkd{";
    STRCMP_EQUAL(nullptr, findOpenCandidate(test));
}

TEST(match_braces, noOpenCandidateIfStringIsNull)
{
    STRCMP_EQUAL(nullptr, findOpenCandidate(nullptr));
}
