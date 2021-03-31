/*
 * two_d_array.h
 *
 *  Created on: Mar 21, 2021
 *      Author: whein
 */

#ifndef _TWO_D_ARRAY_H_
#define _TWO_D_ARRAY_H_

typedef int base_t;

#ifdef DOUBLE_SUBSCR
typedef base_t **arr_t;

#elif defined(SINGLE_SUBSCR_SAVED_COL_SZ)
typedef struct {
    size_t _cols;
    base_t *_a;
} arr_t;

#else
typedef base_t *arr_t;
#endif

extern arr_t alloc2d(size_t rows, size_t cols);
extern void free2d(arr_t arr);

#ifdef UNIT_TEST
#define STATIC_INLINE
#else
#define STATIC_INLINE static inline
#endif


#endif /* _TWO_D_ARRAY_H_ */
