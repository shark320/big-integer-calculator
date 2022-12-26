
#ifndef LONG_ARITHMETIC
#define LONG_ARITHMETIC
#include <stdio.h>

#include "../utils/vector/vector.h"

typedef struct {
    vector* digits;
    short int sign;
}l_number;

l_number* l_number_init ();

l_number* l_number_init (const char* value);

l_number* l_number_copy (const l_number* value);

l_number* l_sum (const l_number* const first, const l_number* const second);

l_number* l_sub (const l_number* const first, const l_number* const second);

l_number* l_div (const l_number* const first, const l_number* const second);

l_number* l_mult (const l_number* const first, const l_number* const second);


#endif