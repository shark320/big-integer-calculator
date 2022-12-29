#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"
#include "utils/expression/expression.h"
#include "calculator/calculator.h"


int main (){
    bigint* number1, *number2,*number3, *result;
    cstring* string1, *string2, *nstring,*token;
    vector* tokens;
    calculator* calc;
    size_t i;
    char* operands = "+-/*%^!";

    string1 = string_init("123+24");

    tokens = expression_parse(string1, operands);

    printf("validated: %d\n", expression_validate(tokens,operands));

    for (i=0;i<vector_count(tokens);++i){
        token = *(cstring**)vector_at(tokens,i);
        printf("%s\n", get_chars(token));

    }

    calc = calc_create();
    result = calc_calculate(calc,tokens);
    nstring = bigint_to_string(result);
    printf("result: %s", get_chars(nstring));
    calc_destroy(&calc);

    string_destroy(&string1);
/*     string_destroy(&string2); */
    string_destroy(&nstring);
    vector_destroy(&tokens);
/*     bigint_destroy(&number1);
    bigint_destroy(&number2);
    bigint_destroy(&number3); */
    bigint_destroy(&result);

    return EXIT_SUCCESS;
}