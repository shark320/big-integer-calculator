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

int string_remove_spaces (cstring* string){
    vector* new_chars;
    size_t len,i;
    char c;

    if (!string){
        return 0;
    }

    len = string_size(string);

    new_chars = vector_create(sizeof(char), NULL);

    if (!new_chars){
        return 0;
    }

    for(i=0;i<len;++i){
        c = string_char_at (string,i);
        if (c!=' ' && c!= '\t'){
            if (!vector_push_back(new_chars, &c)){
                vector_destroy(&new_chars);
                return 0;
            }
        }
    }


    vector_destroy(&(string->chars));
    string->chars = new_chars;
    if (!append_zero(string)){
        return 0;
    }
    return 1;
}

int string_lowercase (cstring* string){
    size_t len,i;
    char c,lc;

    if (!string){
        return 0;
    }

    len = string_size(string);

    for (i=0;i<len;++i){
        c = string_char_at(string,i);
        lc = tolower(c);
        if (c != lc){
            if (!vector_push(string->chars,&lc,i)){
                return 0;
            }
        }
    }

    return 1;
}

int string_isempty (const cstring* string){
    if (!string){
        return 1;
    }

    return string_size(string)<1;
}

int string_clear (cstring* string){
    vector* new_vector;
    if (!string){
        return 0;
    }

    new_vector = vector_create(sizeof(char), NULL);

    if (!new_vector){
        return 0;
    }

    vector_destroy(&string->chars);
    string->chars = new_vector;

    return append_zero(string);
}
