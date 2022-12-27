#include "expression.h"

int contains (const char* characters, char c){
    size_t i;
    if (!characters){
        return 0;
    }

    for (i=0;i<strlen(characters);++i){
        if (characters[i]==c){
            return 1;
        }
    }

    return 0;
}

vector* expression_parse(cstring* string, char* operands){
    size_t string_len;
    size_t i;
    vector* tokens;
    cstring* tmp = string_create();
    char c;

    if (!string){
        return NULL;
    }

    tokens = vector_create(sizeof(cstring*), (vec_it_dealloc_type)string_destroy);

    if (!operands){
        vector_push_back(tokens, string);
        return tokens;
    }

    string_len = string_size(string);

    for(i=0;i<string_len;++i){
        c=string_char_at(string,i);
        if (contains(operands, c)){
            vector_push_back(tokens, &tmp);
            tmp = string_create();
            string_append(tmp,c);
            vector_push_back(tokens,&tmp);
            tmp = string_create();
        }else{
            string_append(tmp,c);
        }
    }
    vector_push_back(tokens, &tmp);
    return tokens;
}