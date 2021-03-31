/**
 *******************************************************************************
 *
 *
 *******************************************************************************
 */

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "two_d_array.h"

#ifdef __USE_MALLOC_REPLACEMENT__
// NOTE: the malloc replacment uses deprecated function sbrk and brk.  These work on linux, but
//       fail to execute properly on a MAC, resulting in UT failures
size_t malloc_buffer_size = 0;

void *my_malloc(size_t size)
{
	void *block_addr = sbrk(size);
	void * updated = sbrk(0);
	malloc_buffer_size += (uintptr_t)updated - (uintptr_t)block_addr;
	return block_addr;
}

void my_free(void *data)
{
	void * curr_top = sbrk(0);
	memset(data, 0, malloc_buffer_size);
	brk(curr_top - malloc_buffer_size);
	void * updated = sbrk(0);
}

#else
void *my_malloc(size_t size) {
    return malloc(size);
}
void my_free(void *data)
{
    free(data);
}
#endif /* __USE_MALLOC_REPLACEMENT__ */

#ifdef DOUBLE_SUBSCR

STATIC_INLINE
void set(arr_t a, size_t i, size_t j, base_t val) {
    a[i][j] = val;
}
#elif defined(SINGLE_SUBSCR_SAVED_COL_SZ)

STATIC_INLINE
void set(arr_t a, size_t i, size_t j, base_t val) {
    a._a[i * a._cols + j] = val;
}
#endif

//arr_t alloc2d(size_t rows, size_t cols);
//void free2d(arr_t arr);

int my_main(void) {
    const size_t N = 3, M = 4;
    arr_t a = alloc2d(N, M);
    for (size_t i = 0; i < N; ++i) {
	for (size_t j = 0; j < M; ++j) {
#if (defined(DOUBLE_SUBSCR) || defined(SINGLE_SUBSCR_SAVED_COL_SZ))
	    set(a, i, j, 0);
#else
	    a[i * M + j] = 0;
#endif
	}
    }
    free2d(a);
    return 0;
}

arr_t alloc2d(size_t rows, size_t cols) {
#ifdef DOUBLE_SUBSCR
	base_t **arr;
	/* row pointers, plus storage for all the elements */
	size_t size = rows*sizeof(base_t *)+cols*rows*sizeof(base_t);
	arr = (base_t **)my_malloc(size);
	base_t *dPtr = (base_t*)(arr+rows);
	for (size_t curRow = 0; curRow < rows; curRow++) {
		arr[curRow] = dPtr+curRow*cols;
	}

    return (arr);
#else
    base_t *a = (base_t *)malloc(rows * cols * sizeof(base_t));
#ifdef SINGLE_SUBSCR_SAVED_COL_SZ
    arr_t arr = { cols, a };
    return arr;
#else
    return a;
#endif
#endif
}

void free2d(arr_t a) {
#ifdef DOUBLE_SUBSCR
    my_free(a);
#elif defined(SINGLE_SUBSCR_SAVED_COL_SZ)
    free(a._a);
#else
    free(a);
#endif
}

#if !defined(UNIT_TEST)
int main(void) {
	return my_main();
}
#endif
