
#include "futils.h"



vector* file_lines(const char* filename){
    FILE* file;
    vector* lines;
    char line[BUFFER_SIZE]; 

     #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            vector_destroy(&lines);   \
            fclose(file);               \
            return NULL;                     \
        }

    if (!filename){
        return NULL;
    }

    file = fopen(filename, "r");

    if (!file){
        return NULL;
    }

    lines = vector_create(BUFFER_SIZE, NULL);

    EXIT_IF_NOT(lines);

    while (fgets(line, BUFFER_SIZE, file)){
        if (strlen(line) == 0){
            continue;
        }
        /*remove trailing characters */
        line[strcspn(line, "\n")] =0;
        EXIT_IF_NOT(vector_push_back(lines,&line))
    }

    #undef EXIT_If_NOT

    return lines;
}