//
//  read4_tests.cpp
//

#include <stdio.h>
#include "CppUTest/TestHarness.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;

extern "C" {
	static unsigned int fake_read4(char *buf);
	static char full_buffer[256];
	static char *buf_index = full_buffer;

	unsigned int read4(char *);
	unsigned int buf_read(char *, unsigned int);
	void reset_read_buffer(void);

	unsigned int fake_read4(char *buf) {
		unsigned int buf_len;
		buf_len = (unsigned int) strlen(buf_index);
		buf_len = (buf_len > 4 ? 4 : buf_len);
		memcpy(buf, buf_index, buf_len);
		buf[buf_len] = '\0';
		buf_index+=buf_len;
		return buf_len;
	}

	FAKE_VALUE_FUNC(unsigned int, read4, char *);
}


TEST_GROUP(buf_read)
{
	char small_buf[4];

    void setup() override
    {
    	FFF_RESET_HISTORY();
    	RESET_FAKE(read4);
    	reset_read_buffer();
		buf_index = full_buffer;
    	read4_fake.custom_fake = fake_read4;
    	sprintf(full_buffer, "beefface");
    }

    void teardown() override
    {
    }
};

TEST(buf_read, read4_mock_returns_first_4_characters)
{
	LONGS_EQUAL(4,read4(small_buf));
	STRCMP_EQUAL("beef", small_buf);
	LONGS_EQUAL(1, read4_fake.call_count);
}

TEST(buf_read, read4_mock_returns_next_4_characters)
{
	LONGS_EQUAL(4,read4(small_buf));
	STRCMP_EQUAL("beef", small_buf);
	LONGS_EQUAL(4,read4(small_buf));
	STRCMP_EQUAL("face", small_buf);
}

TEST(buf_read, read4_mock_will_return_a_partial_buf_when_less_than_4_characters_remaining)
{
	sprintf(full_buffer, "01");
	LONGS_EQUAL(2,read4(small_buf));
	STRNCMP_EQUAL("01", small_buf, sizeof(small_buf));
}

TEST(buf_read, read4_mock_will_return_an_empty_buf_when_no_characters_remaining)
{
	sprintf(full_buffer, "01");
	full_buffer[0]='\0';
	LONGS_EQUAL(0,read4(small_buf));
	STRNCMP_EQUAL("", small_buf, sizeof(small_buf));
}

TEST(buf_read, buf_read_can_read)
{
	sprintf(full_buffer, "abcd");
	LONGS_EQUAL(4, buf_read(small_buf, 4));
	STRNCMP_EQUAL("abcd", small_buf, sizeof(small_buf));
}

TEST(buf_read, buf_read_will_read_in_large_strings)
{
	char large_buf[20];
	sprintf(full_buffer, "abcdfeedface");
	LONGS_EQUAL(12, buf_read(large_buf, 12));
	STRCMP_EQUAL("abcdfeedface", large_buf);
	LONGS_EQUAL(3, read4_fake.call_count);
}

TEST(buf_read, can_read_a_large_string_with_a_partial_last_read)
{
	char large_buf[20];
	sprintf(full_buffer, "abcdefghijk");
	LONGS_EQUAL(11, buf_read(large_buf, 20));
	STRCMP_EQUAL("abcdefghijk", large_buf);
	LONGS_EQUAL(3, read4_fake.call_count);
}

TEST(buf_read, buf_read_will_read_in_small_strings)
{
	sprintf(full_buffer, "a");
	LONGS_EQUAL(1, buf_read(small_buf, 1));
	STRCMP_EQUAL("a", small_buf);
}

TEST(buf_read, reading_in_small_strings_will_be_handled_with_a_single_read4_call)
{
	sprintf(full_buffer, "abc");
	LONGS_EQUAL(1, buf_read(small_buf, 1));
	STRCMP_EQUAL("a", small_buf);
	LONGS_EQUAL(1, buf_read(small_buf, 1));
	STRCMP_EQUAL("b", small_buf);
	LONGS_EQUAL(1, read4_fake.call_count);
}

TEST(buf_read, attempting_a_new_read_after_buffer_depleted_returns_a_null_buffer)
{
	sprintf(full_buffer, "abc");
	LONGS_EQUAL(3, buf_read(small_buf, 4));
	STRCMP_EQUAL("abc", small_buf);
	LONGS_EQUAL(0, buf_read(small_buf, 1));
	STRCMP_EQUAL("", small_buf);
}

TEST(buf_read, attempting_an_invalid_small_read_after_multiple_small_reads_returns_a_null_buffer)
{
	sprintf(full_buffer, "abc");
	LONGS_EQUAL(1, buf_read(small_buf, 1));
	STRCMP_EQUAL("a", small_buf);
	LONGS_EQUAL(2, buf_read(small_buf, 2));
	STRCMP_EQUAL("bc", small_buf);
	LONGS_EQUAL(0, buf_read(small_buf, 1));
	STRCMP_EQUAL("", small_buf);
}
