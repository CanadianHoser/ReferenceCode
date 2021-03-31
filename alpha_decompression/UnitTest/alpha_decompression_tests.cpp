//
//  2d-array_tests.cpp
//
//

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include <string.h>

extern "C" {
extern char *find_next_decompress_token(char const *comp_str, int *index);
extern int expandBy(char const *comp_str);
extern char * expandBuffer(char *buffer, int bufferSize, char const *comp_str);
extern char *decompress(char const *comp_str, char *output_buffer,
    size_t output_buffer_size);
extern int my_main();
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

// These are used to validate my understanding of the isalpha() family of functions.
// They would be removed if this were production code, but retained to show how I use
// the UT infra to confirm my understanding of unfamiliar functios.
TEST_GROUP(is_alpha_tests)
{
    void setup() { }
    void teardown() { }
};

TEST(is_alpha_tests, letter_is_alpha)
{
	CHECK_TRUE(isalpha('c'));
}

TEST(is_alpha_tests, number_is_not_alpha)
{
	CHECK_TRUE(!isalpha('2'));
}

TEST(is_alpha_tests, number_is_digit)
{
	CHECK_TRUE(isdigit('2'));
}

TEST(is_alpha_tests, letter_is_not_digit)
{
	CHECK_TRUE(!isdigit('a'));
}

TEST(is_alpha_tests, isalpha_on_char_works)
{
	char letter = 'a';
	CHECK_TRUE(!isdigit(letter));
}

TEST(is_alpha_tests, isalpha_on_word_works)
{
	char const *word = "Hello";
	CHECK_TRUE(!isdigit(*word));
}

TEST_GROUP(find_decompress_token)
{
	int index;
    void setup() { index = 0;}
    void teardown() { }
};

TEST(find_decompress_token, find_first_compression_key)
{
	STRCMP_EQUAL("o2World", find_next_decompress_token("Hello2World", &index));
	LONGS_EQUAL(4, index);
}

TEST(find_decompress_token, no_compression_key_returns_null_string)
{
	STRCMP_EQUAL(NULL, find_next_decompress_token("HelloWorld", &index));
	LONGS_EQUAL(10, index);
}

TEST(find_decompress_token, first_digit_is_numeric_returns_null_string)
{
	STRCMP_EQUAL("", find_next_decompress_token("2HelloWorld", &index));
	LONGS_EQUAL(0, index);
}

TEST(find_decompress_token, first_digit_is_numeric_with_embedded_token_returns_null_string)
{
	STRCMP_EQUAL("", find_next_decompress_token("2Hello3World", &index));
	LONGS_EQUAL(0, index);
}

TEST_GROUP(expandBy)
{
    void setup() { }
    void teardown() { }
};

TEST(expandBy, valid_expansion_is_evaluated)
{
	LONGS_EQUAL(3, expandBy("o3"));
}

TEST(expandBy, expansion_by_value_greater_than_10)
{
	LONGS_EQUAL(12, expandBy("c12"));
}

TEST(expandBy, invalid_expansion_returns_error_value)
{
	LONGS_EQUAL(-1, expandBy("33"));
}

TEST(expandBy, full_expansion_is_possible)
{
	LONGS_EQUAL(26, expandBy("a26"));
}

TEST(expandBy, expansion_out_of_range_causes_error)
{
	LONGS_EQUAL(-2, expandBy("x4"));
}

TEST_GROUP(expandBuffer)
{
	char buffer[32];
    void setup() { memset(buffer, 0, 32);}
    void teardown() { }
};

TEST(expandBuffer, simpleExpansion)
{
	STRCMP_EQUAL("abc", expandBuffer(&buffer[0], 32, "a3"));
}

TEST(expandBuffer, simpleExpansion_does_not_overrun_buffer)
{
	STRCMP_EQUAL("ab", expandBuffer(&buffer[0], 3, "a3"));
}

TEST(expandBuffer, simpleExpansion_does_not_add_beyond_token)
{
	STRCMP_EQUAL("abc", expandBuffer(&buffer[0], 32, "a3llo"));
}

TEST(expandBuffer, zero_expansion_buffer_results_in_NULL)
{
	STRCMP_EQUAL(NULL, expandBuffer(&buffer[0], 0, "a3llo"));
}

TEST_GROUP(decompress)
{
	char buffer[256];
    void setup() { memset(buffer, 0, 256);}
    void teardown() { }
};

TEST(decompress, no_decompression_returns_same_string)
{
	STRCMP_EQUAL("HelloWorld", decompress("HelloWorld", buffer, 256));
}

TEST(decompress, decompression_at_beginning_evaluates_correctly)
{
	STRCMP_EQUAL("abcHelloWorld", decompress("a3HelloWorld", buffer, 256));
}

TEST(decompress, decompression_at_middle_evaluates_correctly)
{
	STRCMP_EQUAL("HelloabcWorld", decompress("Helloa3World", buffer, 256));
}

TEST(decompress, decompression_at_end_evaluates_correctly)
{
	STRCMP_EQUAL("HelloWorldxyz", decompress("HelloWorldx3", buffer, 256));
}

TEST(decompress, multiple_decompression_evaluates_correctly)
{
	STRCMP_EQUAL("abcHelloWorldxyz", decompress("a3HelloWorldx3", buffer, 256));
}

TEST(decompress, dest_buffer_too_small_returns_NULL)
{
	char smallbuf[4] = {};
	STRCMP_EQUAL(NULL, decompress("HelloWorld", smallbuf, 4));
}

TEST(decompress, dest_buffer_too_small_after_expansion_returns_NULL)
{
	char smallbuf[8] = {};
	STRCMP_EQUAL(NULL, decompress("a3Hello", smallbuf, 8));
}

TEST(decompress, dest_buffer_too_small_after_expansion_at_end_returns_NULL)
{
	char smallbuf[8] = {};
	STRCMP_EQUAL(NULL, decompress("HelloWa3", smallbuf, 8));
}

TEST(decompress, dest_buffer_must_have_space_for_NULL_at_end_of_decompressed_string)
{
	char smallbuf[9] = {};
	STRCMP_EQUAL(NULL, decompress("HelloWa3", smallbuf, 9));
}

TEST(decompress, dest_buffer_must_have_NULL_at_end_of_decompressed_string)
{
	char smallbuf[10] = {};
	smallbuf[9] = '@';
	STRCMP_EQUAL("HelloWabc", decompress("HelloWa3", smallbuf, 10));
	CHECK_TRUE(smallbuf[9] == '\0');
}

TEST(decompress, expansion_token_beyond_buffer_bounds_results_in_NULL)
{
	char smallbuf[4] = {};
	STRCMP_EQUAL(NULL, decompress("HelloWa3", smallbuf, 4));
}

TEST(decompress, atmosic_decompression_string)
{
    char decompress_this[] = "c3j3d8js3bsj2k4bo3k";
	STRCMP_EQUAL("cdejkldefghijkjstubsjkklmnbopqk", decompress(decompress_this, buffer, 256));
	printf("[ %s ] decompressed = [ %s ]\n", decompress_this, buffer);
}

TEST(decompress, run_my_main_for_code_coverage)
{
	LONGS_EQUAL(0, my_main());
}
