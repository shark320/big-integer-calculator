#include <stdio.h>
#include <stdlib.h>
#include "long_arithmetic/long_arithmetic.h"
#include "utils/vector/vector.h"
#include "utils/futils/futils.h"
#include "utils/cstring/cstring.h"


int main (){
    vector* lines;
    size_t i;
    char* line;
    cstring* string = string_init("test");
    lines = file_lines("../data/test.txt");

    printf("size of vector: %ld \n", vector_count(lines));

    for (i=0;i<vector_count(lines);i++){
        line =  ((char*)vector_at(lines,i));
        printf("line %s \n",line);
    }


    string_append(string, 'a');
    printf("string: %s", get_chars(string));

    vector_destroy(&lines);
    vector_destroy(&string->chars);
    free(string);

    return EXIT_SUCCESS;
}