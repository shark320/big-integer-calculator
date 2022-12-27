
#ifndef LONG_ARITHMETIC
#define LONG_ARITHMETIC
#include <stdio.h>

#include "../utils/vector/vector.h"
#include "../utils/cstring/cstring.h"

#define POSITIVE 1
#define NEGATIVE -1

typedef struct {
    vector* digits;
    signed char sign;
}bigint;

bigint* bigint_cretate ();

bigint* bigint_init (const cstring* value, const int sign);

void bigint_destroy (bigint** value);

signed char bigint_digit_at (const bigint* value, size_t index);

bigint* bigint_copy (const bigint* value);

size_t bigint_size (const bigint* value);

bigint* l_sum (const bigint*  first, const bigint* second);

bigint* l_sub (const bigint*  first, const bigint*  second);

bigint* l_div (const bigint*  first, const bigint*  second);

bigint* l_mult (const bigint*  first, const bigint*  second);

cstring* bigint_to_string(const bigint* value);

#endif