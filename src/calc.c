#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"
#include "utils/expression/expression.h"


int main (){
    vector* tokens;
    size_t i;
    char operands[] = {'+','*','-','/'};
    cstring* string = string_init("123");

    tokens = expression_parse(string,operands);
    for(i=0;i<vector_count(tokens);++i){
        char* c = get_chars(*((cstring**)vector_at(tokens,i)));
        printf("%s\n",c);
    }
    for(i=0;i<vector_count(tokens);++i){
        printf("%s\n",get_chars(*((cstring**)vector_at(tokens,i))));
    }

    
    vector_destroy(&tokens);
    string_destroy(&string);

    return EXIT_SUCCESS;
}