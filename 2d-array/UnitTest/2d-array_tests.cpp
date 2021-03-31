//
//  2d-array_tests.cpp
//
//

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

extern "C" {
#include "two_d_array.h"
    extern int my_main(void);
    extern void * my_malloc(int);
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

TEST_GROUP(2d_array_tests)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(2d_array_tests, sanity_check)
{
    LONGS_EQUAL(3, 1+2);
}

TEST(2d_array_tests, call_main)
{
	LONGS_EQUAL(0, my_main());
}

extern "C" {
extern size_t malloc_buffer_size;
}

TEST(2d_array_tests, get_current_allocation_break)
{
	CHECK_TRUE(NULL != my_malloc(0));
}

TEST_GROUP(my_malloc_tests)
{
	void *init_sbrk;

    void setup()
    {
		init_sbrk = sbrk(0);
    }

    void teardown()
    {
    	brk(init_sbrk);
    	malloc_buffer_size = 0;
    }
};

TEST(my_malloc_tests, my_malloc_increments_sbrk_address)
{
	void *updated_sbrk = my_malloc(0x10);
	updated_sbrk = sbrk(0);
	LONGS_EQUAL((uintptr_t)init_sbrk+0x10, updated_sbrk);
	LONGS_EQUAL(0x10, malloc_buffer_size);
}

TEST(my_malloc_tests, my_malloc_with_negative_value_decrements_sbrk_address)
{
	void * updated_sbrk = my_malloc(0x10);
	updated_sbrk = sbrk(0);
	LONGS_EQUAL((uintptr_t)init_sbrk+0x10, updated_sbrk);
	// MacOS Note: this fails, as the negative value fails to register in their implementation.
	// Additionally, trying to reprogram it using brk(init_sbrk) also fails.  Testing passes
	// using Ubuntu 5.8.0-45.
	updated_sbrk = sbrk(-0x10);
	updated_sbrk = sbrk(0);
	LONGS_EQUAL(init_sbrk, updated_sbrk);
}

TEST(my_malloc_tests, my_malloc_with_zero_size_does_not_increase_memory_usage)
{
	void * updated_sbrk = my_malloc(0x0);
	updated_sbrk = sbrk(0);
	LONGS_EQUAL(init_sbrk, updated_sbrk);
	LONGS_EQUAL(0x0, malloc_buffer_size);
}

TEST_GROUP(alloc_2d)
{
	void *init_sbrk;

	void setup() {
		init_sbrk = sbrk(0);
	}
	void teardown() {
    	brk(init_sbrk);
    	malloc_buffer_size = 0;
	}
};

TEST(alloc_2d, test)
{
	// This doesn't do anything yet
	arr_t arr = alloc2d(3, 4);
	const int rows = 3;
	const int cols = 4;
	arr[2][3] = 0x23;
	arr[0][0] = 0;
	arr[1][0] = 0x10;
	LONGS_EQUAL(0x23,arr[2][3]);
	LONGS_EQUAL(0x0,arr[0][0]);
	LONGS_EQUAL(0x10, *((base_t *)(arr+rows)+cols*1+0));
	LONGS_EQUAL(0x23, *((base_t *)(arr+rows)+cols*2+3));
	POINTERS_EQUAL(arr+rows, &arr[0][0]);
	POINTERS_EQUAL(arr[0], &arr[0][0]);
	POINTERS_EQUAL(arr[1], &arr[1][0]);
	POINTERS_EQUAL(arr[2], &arr[2][0]);
	POINTERS_EQUAL((((base_t*)(arr+rows))+cols*1+0), &arr[1][0]);
}
