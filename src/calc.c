#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"
#include "utils/expression/expression.h"


int main (){
    bigint* number1, *number2,*number3, *result;
    cstring* string1, *string2, *nstring;

    string1 = string_init("108");

    string2 = string_init("923");

    number1 = bigint_init(string1, POSITIVE);

    number2 = bigint_init(string2, POSITIVE);

    number3 = bigint_get_n_first_digits(number1, 0);

    result = l_mult(number1, number2);

    l_mult_assign(&number1, number2);
    l_mult_assign(&number1, number2);

    nstring = bigint_to_string(number3);

    printf("number: %s\n", get_chars(nstring));

    printf("%d\n", bigint_cmp(number1,number2));

    string_destroy(&string1);
    string_destroy(&string2);
    string_destroy(&nstring);
    bigint_destroy(&number1);
    bigint_destroy(&number2);
    bigint_destroy(&number3);
    bigint_destroy(&result);

    return EXIT_SUCCESS;
}