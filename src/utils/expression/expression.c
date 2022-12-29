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

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            string_destroy(&tmp);   \
            vector_destroy(&tokens); \
            return NULL;                     \
        }

    if (!string){
        return NULL;
    }

    tokens = vector_create(sizeof(cstring*), (vec_it_dealloc_type)string_destroy);

    if (!operands){
        EXIT_IF_NOT(vector_push_back(tokens, string));
        return tokens;
    }

    string_len = string_size(string);

    for(i=0;i<string_len;++i){
        c=string_char_at(string,i);
        if (contains(operands, c)){
            if (!string_isempty(tmp)){
                EXIT_IF_NOT(string_lowercase(tmp));
                EXIT_IF_NOT(vector_push_back(tokens, &tmp));
            }else{
                string_destroy(&tmp);
            }
            tmp = string_create();
            EXIT_IF_NOT(string_append(tmp,c));
            EXIT_IF_NOT(string_lowercase(tmp));
            EXIT_IF_NOT(vector_push_back(tokens,&tmp));
            tmp = string_create();
        }else{
            EXIT_IF_NOT(string_append(tmp,c));
        }
    }
    if (!string_isempty(tmp)){
        EXIT_IF_NOT(string_lowercase(tmp));
        EXIT_IF_NOT(vector_push_back(tokens, &tmp));
    }else{
        string_destroy(&tmp);
    }
    return tokens;
}

int validate_bin (cstring* number){
    size_t len,i;
    char c;

    if (!number){
        return 0;
    }

    len = string_size(number);

    for (i=2;i<len;++i){
        c=string_char_at(number,i);
        if ( !(c=='1' || c=='0')){
            return 0;
        }
    }

    return 1;
}

int validate_hex (cstring* number){
    size_t len,i;
    char c;

    if (!number){
        return 0;
    }

    len = string_size(number);

    for (i=2;i<len;++i){
        c=string_char_at(number,i);
        if ( ! ((c>='0' && c<='9') || (c>='a' && c<='f'))){
            return 0;
        }
    }

    return 1;
}

int validate_dec (cstring* number){
    size_t len,i;
    char c;

    if (!number){
        return 0;
    }

    len = string_size(number);

    for (i=0;i<len;++i){
        c=string_char_at(number,i);
        if (c<'0' || c>'9'){
            return 0;
        }
    }

    return 1;
}

int validate_number (cstring* number){
    size_t len;
    char c;
    if (!number){
        return 0;
    }

    len = string_size(number);

    if (len>=3){
        c = string_char_at(number,0);
        if (c=='0'){
            c = string_char_at(number,1);
            if (c=='x'){
                return validate_hex(number);
            }
            if (c=='b'){
                return validate_bin(number);
            }
        }
    }

    return validate_dec(number);
}

int expression_validate(vector* tokens, char* operands){
    size_t len,i;
    cstring* token;
    if (!tokens){
        return 0;
    }

    len = vector_count (tokens);

    for (i=0;i<len;++i){
        token = *(cstring**)vector_at(tokens,i);
        if (!token){
            return 0;
        }
        if (string_size(token)==1){
            if (contains(operands, string_char_at(token,0))){
                continue;
            }
        }
        if (!validate_number(token)){
            return 0;
        }
    }

    return 1;
}