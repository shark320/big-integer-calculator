#ifndef CALCULATOR
#define CALCULATOR

#include "../utils/vector/vector.h"
#include "../long_arithmetic/long_arithmetic.h"

typedef bigint* (*l_operator)(vector* stack);

typedef struct {
    vector* values;
    vector* operators;
} calculator;


calculator* calc_create();

void calc_destroy(calculator** instance);

bigint* calc_calculate(calculator* calc ,const vector* tokens);

bigint* calc_sum(vector* values);

#endif