#ifndef CALCULATOR
#define CALCULATOR

#include "../utils/vector/vector.h"
#include "../long_arithmetic/long_arithmetic.h"
#include "../utils/expression/expression.h"
#include "../utils/errors/errors.h"

typedef bigint* (*operator_handler)(vector* stack, int* error);

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

bigint* calc_calculate(calculator* calc ,const vector* tokens, int* error);

bigint* calc_sum(vector* values, int* error);

bigint* calc_mult(vector* values, int* error);

bigint* calc_sub(vector* values, int* error);

bigint* calc_div(vector* values, int* error);

bigint* calc_mod(vector* values, int* error);

bigint* calc_pow(vector* values, int* error);

bigint* calc_negate(vector* values, int* error);

bigint* calc_fact(vector* values, int* error);

#endif