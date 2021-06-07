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
	extern bool isMatchingCandidate(char matchTo, char candidate);
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

TEST(match_braces, isOpenCandidateInStringStartingWithBrace)
{
	const char *test="{jfkd";
	CHECK_TRUE(isCandidateOpen(*test));
}

TEST(match_braces, notOpenCandidateInStringStartingWithCharacter)
{
	const char *test="jfkd";
	CHECK_FALSE(isCandidateOpen(*test));
}

TEST(match_braces, cannotFindOpenCandidateInStringWithOnlyASingleBrace)
{
	const char *test="[";
	CHECK_FALSE(findOpenCandidate(test));
}

TEST(match_braces, noOpenCandidateIfStringIsNull)
{
    STRCMP_EQUAL(nullptr, findOpenCandidate(nullptr));
}

TEST(match_braces, noOpenCandidateForEmptyString)
{
    STRCMP_EQUAL(nullptr, findOpenCandidate(""));
}

TEST(match_braces, canFindNextOpenCandidateAtStartOfString)
{
    STRCMP_EQUAL("{jfkd", findOpenCandidate("{jfkd"));
}

TEST(match_braces, canFindNextOpenCandidateInMiddleOfString)
{
    STRCMP_EQUAL("{kd", findOpenCandidate("jf{kd"));
}

TEST(match_braces, shouldNotFindOpenCandidateAtEndOfString)
{
    STRCMP_EQUAL(nullptr, findOpenCandidate("jfkd{"));
}

TEST(match_braces, findSimpleMatch)
{
	CHECK_TRUE(bracesComplete("{}"));
}

TEST(match_braces, findNoMatchInMiddle)
{
	CHECK_FALSE(bracesComplete("{{}"));
}

TEST(match_braces, findNoMatch)
{
	CHECK_FALSE(bracesComplete("{jk"));
}

TEST(match_braces, findSimpleMatchWithMiddleChars)
{
	CHECK_TRUE(bracesComplete("[adb]"));
}

TEST(match_braces, findSimpleMatchWithCharsAtEnd)
{
	CHECK_TRUE(bracesComplete("[adb]sdf"));
}

TEST(match_braces, findSimpleMatchWithCharsAtBeginning)
{
	CHECK_TRUE(bracesComplete("jk[adb]"));
}

TEST(match_braces, findMultipleMatches)
{
	CHECK_TRUE(bracesComplete("{[]}"));
}

TEST(match_braces, interleavedBracesDoNotMatch)
{
	CHECK_FALSE(bracesComplete("{[}]"));
}

TEST(match_braces, noBracesInStringShouldMatch)
{
	CHECK_TRUE(bracesComplete("abcd"));
}

TEST(match_braces, emptyStringShouldMatch)
{
	CHECK_TRUE(bracesComplete(""));
}

TEST(match_braces, nullStringShouldMatch)
{
	CHECK_TRUE(bracesComplete(nullptr));
}
