#include "cstring.h"

int append_zero (cstring* string){
    char zero = '\0';
    if (!string || !string->chars){
        return 0;
    }

    return vector_push_back(string->chars, &zero);
}

cstring* string_create (){
    cstring* new_string;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            string_destroy(&new_string);   \
            return NULL;                     \
        }

    new_string = (cstring*)malloc(sizeof(cstring));

    if (!new_string){
        return NULL;
    }

    new_string->chars = vector_create(sizeof(char), NULL);

    if (!new_string->chars){
        free(new_string);
        return NULL;
    }

    EXIT_IF_NOT(append_zero(new_string));


    #undef EXIT_IF_NOT
    return new_string;
}

cstring* string_init (const char* value){
    cstring* new_string = string_create();
    size_t i;

    for (i=0;i<strlen(value);++i){
       string_append(new_string,value[i]);
    }
    
    return new_string;
}

char* get_chars(const cstring* string){
    if (!string){
        return NULL;
    }
    return (char*)string->chars->data;
}

void string_destroy(cstring** string){
    if (!string || !*string){
        return;
    }

    vector_destroy(&((*string)->chars));
    free(*string);
    *string = NULL;
}

void string_destroy_t(void** string){
    string_destroy((cstring**)string);
}

char* reverce_chars (char* chars){
    size_t len;
    char* reversed;
    size_t i;
    if (!chars){
        return NULL;
    }

    len = strlen(chars);
    reversed = malloc(sizeof(char)*(len+1));

    if (!reversed){
        free(chars);
        return NULL;
    }

    for (i=0;i<len;++i){
        reversed[i] = chars[len-i-1];
    }

    reversed[len] = 0;

    return reversed;
}

int string_reverse(cstring** string){
    char* reversed;

    if (!string || !*string){
        return 1;
    }

    reversed = reverce_chars(get_chars((*string)));

    string_destroy(string);
    *string = string_init(reversed);
    free(reversed);

    if (!*string){
        return 1;
    }

    return 0;
}

size_t string_size(const cstring* string){
    if (!string || !string->chars){
        return 0;
    }

    return vector_count(string->chars)-1;
}

int string_append (cstring* string, const char value){
    if (!string){
        return 0;
    }
    if (vector_push(string->chars, &value,string_size(string))!=1){
        return 0;
    }
    return append_zero(string);
}

char string_char_at(const cstring* string, size_t index){
    char* c;

    if (!string ||index>=string_size(string)){
        return 0;
    }

    c = (char*)vector_at(string->chars, index);

    return c==NULL?0:*c;
}
