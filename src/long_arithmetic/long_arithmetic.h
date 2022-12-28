
#ifndef LONG_ARITHMETIC
#define LONG_ARITHMETIC
#include <stdio.h>
#include <limits.h>

#include "../utils/vector/vector.h"
#include "../utils/cstring/cstring.h"

#define POSITIVE 1
#define NEGATIVE -1

#define LESS -1
#define GREATER 1
#define EQUALS 0

typedef struct {
    vector* digits;
    signed char sign;
}bigint;

bigint* bigint_cretate ();

bigint* bigint_zero();

bigint* bigint_init (const cstring* value, const short int sign);

bigint* bigint_init_chars (const char* value, const short int sign);

void bigint_destroy (bigint** value);

signed char bigint_digit_at (const bigint* value, size_t index);

int bigint_cmp (const bigint* first, const bigint* second);

int bigint_cmp_abs (const bigint* first, const bigint* second);

bigint* bigint_copy (const bigint* value);

bigint* bigint_copy_abs (const bigint* value);

size_t bigint_size (const bigint* value);

bigint* l_sum_abs (const bigint*  first, const bigint* second);

bigint* l_sum (const bigint*  first, const bigint* second);

bigint* l_sub (const bigint*  first, const bigint*  second);

bigint* l_sub_abs (const bigint*  first, const bigint*  second);

bigint* l_div (const bigint*  first, const bigint*  second);

bigint* l_mult_abs (const bigint*  first, const bigint*  second);

bigint* l_mult (const bigint*  first, const bigint*  second);

bigint* bigint_get_n_first_digits (const bigint* value, size_t n);

int l_mult_assign (bigint** first, const bigint*  second);

cstring* bigint_to_string(const bigint* value);

int bigint_trim (const bigint* value);

#endif