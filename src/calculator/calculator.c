#include "calculator.h"

calculator* calc_create(){
    calculator* instance;

    instance = (calculator*)malloc(sizeof(calculator));

    if (!instance){
        return NULL;
    }

    instance->operators = vector_create(sizeof(l_operator), NULL);

    if (!instance->operators){
        free(instance);
        return NULL;
    }

    instance->values = vector_create(sizeof(bigint*), (vec_it_dealloc_type)bigint_destroy);

    if (!instance->values){
        vector_destroy(&instance->operators);
        free(instance);
        return NULL;
    }

    return instance;
}

void calc_destroy(calculator** instance){
    if (!instance || !*instance){
        return;
    }

    vector_destroy(&(*instance)->operators);
    vector_destroy(&(*instance)->values);

    free(*instance);
    *instance=NULL;
}

bigint* calc_sum(vector* values){
    bigint** first, **second, *result;
    if (!values){
        return NULL;
    }

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_sum(*first,*second);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_calculate(calculator* calc ,const vector* tokens){
    size_t len,i;
    cstring* token;
    bigint* value;
    l_operator operator;
    char c;
    if (!tokens || !calc){
        return NULL;
    }

    len = vector_count(tokens);

    for (i=0;i<len;++i){
        token = *(cstring**)vector_at(tokens,i);
        if (string_size(token)==1){
            c = string_char_at(token,0);
            if (c == '+'){
                vector_push_back(calc->operators, &calc_sum);
                continue;
            }
        }
        value = bigint_init(token);
        if (!value){
            /* stop function -> error */
        }
        vector_push_back(calc->values, &value);
    }

    operator = vector_pop_last(calc->operators);
    return (operator)(calc->values);
}