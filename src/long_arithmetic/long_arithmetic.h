
#ifndef LONG_ARITHMETIC
#define LONG_ARITHMETIC
#include <stdio.h>
#include <limits.h>

#include "../utils/vector/vector.h"
#include "../utils/cstring/cstring.h"
#include "../utils/errors/errors.h"

#define POSITIVE 1
#define NEGATIVE -1

#define LESS -1
#define GREATER 1
#define EQUALS 0

typedef struct {
    vector* digits;
    signed char sign;
}bigint;

bigint* bigint_create ();

bigint* bigint_zero();

bigint* bigint_init (const cstring* value);

bigint* bigint_init_chars (const char* value);

bigint* bigint_init_long ( long value);

bigint* bigint_init_vector(vector* value, signed char sign);

long bigint_to_long (const bigint* value);

vector* bigint_to_bytes (const bigint* value);

void bigint_destroy (bigint** value);

signed char bigint_digit_at (const bigint* value, size_t index);

int bigint_cmp (const bigint* first, const bigint* second);

int bigint_cmp_abs (const bigint* first, const bigint* second);

bigint* bigint_copy (const bigint* value);

bigint* bigint_copy_abs (const bigint* value);

size_t bigint_size (const bigint* value);

bigint* l_sum_abs (const bigint*  first, const bigint* second);

bigint* l_sum (const bigint*  first, const bigint* second);

int l_sum_assign (bigint**  first, const bigint* second);

bigint* l_sub (const bigint*  first, const bigint*  second);

bigint* l_sub_abs (const bigint*  first, const bigint*  second);

int l_sub_assign (bigint**  first, const bigint*  second);

int l_div_simple (const bigint* first, const bigint* second);

bigint* l_div_abs (const bigint*  first, const bigint*  second,bigint** mod_abs);

bigint* l_div (const bigint*  first, const bigint*  second,bigint** mod_abs);

int l_div_assign (bigint** first, const bigint* second,bigint** mod_abs);

bigint* l_mult_abs (const bigint*  first, const bigint*  second);

bigint* l_mult (const bigint*  first, const bigint*  second);

int l_mult_assign (bigint** first, const bigint*  second);

bigint* l_mult_long (const bigint*  first, const long second);

bigint* l_mod (const bigint* first, const bigint* second);

bigint* l_pow (const bigint* first, const bigint* second,int* error);

bigint* l_pow_long (const bigint* first, const long second, int* error);

bigint* l_fact(const bigint* value, int* error);

int l_inc(bigint* value);

int bigint_is_zero(const bigint* value);

int bigint_is_one (const bigint* value);

bigint* bigint_get_n_first_digits (const bigint* value, size_t n);

cstring* bigint_to_string(const bigint* value);

int bigint_trim (const bigint* value);

bigint* bigint_negate(bigint* value);

#endif