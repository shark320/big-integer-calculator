#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"
#include "utils/expression/expression.h"
#include "calculator/calculator.h"
#include "utils/errors/errors.h"

#define PROMPT ">\n"

#define DEC_CMD "dec"
#define BIN_CMD "bin"
#define HEX_CMD "hex"
#define OUT_CMD "out"
#define QUIT_CMD "quit"

cstring* read_line_cmp(){
    char c;
    cstring* string;
    

    string = string_create();
    if (!string){
        return NULL;
    }

    while( (c = getchar) != '\n'){
        if(!string_append(string,c)){
            string_destroy(&string);
            return NULL;
        }
    }

    return string;
}

int process_command(cstring* command){
    if (!command){
        return;
    }

    if
}

void print_error(int error, cstring* command){
    if (error == ZERO_DIVISION){
        printf("Division by zero!\n");
    }else if (error == NEGATIVE_FACT){
        printf("Input of factorial must not be negative!\n");
    }else if (error == SYNTAX_ERROR){
        printf("Syntax error!\n");
    }else if (error == INVALID_COMMAND){
        if (!command){
            return;
        }
        printf("Invalid command \"%s\"!", get_chars(command));
    }else{
        printf("Unknown error");
    }
}

int calculate (FILE* file){
    cstring* input;
    calculator* calc;
    vector* tokens;
    bigint* result;
    char* operands = "+-/*%^!()";
    int error;
    int out = DEC;

    calc = calc_create();

    if (!calc){
        return;
    }

    for(;;){
        error =0;

        printf(PROMPT);

        if (!file){
            input = read_line_cmp();
        }

        if (!input){
            return;
        }

        tokens = expression_parse(input,operands);

        if (!tokens){
            string_destroy(&input);
            calc_destroy(&calc);
            return;
        }

        result = calc_calculate(calc,tokens,&error);

        if (!result){
            if (error!=0){
                print_error(error,NULL);
            }else{
                return;
            }
        }

    }
}


int main (){
    bigint* number1, *number2,*number3, *result, *result2;
    cstring* string1, *string2, *nstring,*token;
    vector* tokens,*bytes;
    calculator* calc;
    size_t i;
    int error= 0;
    unsigned char d;
    char* operands = "+-/*%^!()";

    string1 = string_init("10+10+");

    string2 = string_init("0xf8");

    printf("is expression: %d\n",is_expression(string1, operands));

    tokens = expression_parse(string1, operands);

/*     printf("validated: %d\n", expression_validate(tokens,operands));

    for (i=0;i<vector_count(tokens);++i){
        token = *(cstring**)vector_at(tokens,i);
        printf("%s\n", get_chars(token));

    } */

    calc = calc_create();
    result = calc_calculate(calc,tokens, &error);
    printf("error: %d\n",error);
/*     result = get_bigint(string2); */
/*     result2 = calc_calculate(calc,tokens); */

    print_bin(result);
    print_hex(result);
    print_dec(result);
    calc_destroy(&calc);

    string_destroy(&string1);
    string_destroy(&string2);
/*     string_destroy(&nstring); */
    vector_destroy(&tokens);
    vector_destroy(&bytes);
/*     bigint_destroy(&number1);
    bigint_destroy(&number2);
    bigint_destroy(&number3); */
    bigint_destroy(&result);
/*     bigint_destroy(&result2); */

    return EXIT_SUCCESS;
}