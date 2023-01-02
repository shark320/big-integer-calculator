#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"
#include "utils/expression/expression.h"
#include "calculator/calculator.h"


int main (){
    bigint* number1, *number2,*number3, *result, *result2;
    cstring* string1, *string2, *nstring,*token;
    vector* tokens,*bytes;
    calculator* calc;
    size_t i;
    unsigned char d;
    char* operands = "+-/*%^!()";

    string1 = string_init("143788010446775248848237875203163336494653562343841/10");

    string2 = string_init("0xb");

    tokens = expression_parse(string1, operands);

/*     printf("validated: %d\n", expression_validate(tokens,operands));

    for (i=0;i<vector_count(tokens);++i){
        token = *(cstring**)vector_at(tokens,i);
        printf("%s\n", get_chars(token));

    } */

    calc = calc_create();
/*     result = calc_calculate(calc,tokens); */
    result = get_bigint(string2);
/*     result2 = bigint_negate(result); */
    bytes = bigint_to_bytes(result);
    nstring = bigint_to_string(result);
    printf("result: %s\n", get_chars(nstring));
    print_bin(result);
    calc_destroy(&calc);

    string_destroy(&string1);
    string_destroy(&string2);
    string_destroy(&nstring);
    vector_destroy(&tokens);
    vector_destroy(&bytes);
/*     bigint_destroy(&number1);
    bigint_destroy(&number2);
    bigint_destroy(&number3); */
    bigint_destroy(&result);
/*     bigint_destroy(&result2); */

    return EXIT_SUCCESS;
}