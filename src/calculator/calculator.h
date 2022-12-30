#ifndef CALCULATOR
#define CALCULATOR

#include "../utils/vector/vector.h"
#include "../long_arithmetic/long_arithmetic.h"

typedef bigint* (*operator_handler)(vector* stack);

typedef struct {
    vector* values;
    vector* operators;
} calculator;

typedef struct {
    operator_handler handler;
    char pecedence;
    char is_open_bracket;
} l_operator;

l_operator* operator_init(const char op, const char is_negate);

l_operator* operator_create(operator_handler handler, const char precedence, const char is_open_bracket);

void operator_destroy(l_operator** op);


calculator* calc_create();

void calc_destroy(calculator** instance);

bigint* calc_calculate(calculator* calc ,const vector* tokens);

bigint* calc_sum(vector* values);

bigint* calc_mult(vector* values);

bigint* calc_sub(vector* values);

bigint* calc_div(vector* values);

bigint* calc_mod(vector* values);

bigint* calc_pow(vector* values);

bigint* calc_negate(vector* values);

bigint* calc_fact(vector* values);

#endif