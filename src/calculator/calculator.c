#include "calculator.h"

l_operator* operator_init(const char op, const char is_negate){
    l_operator* instance;


    if (op=='+'){
        instance = operator_create((operator_handler)calc_sum, 1,0);
    }else if (op=='-'){
        if (is_negate){
            instance = operator_create((operator_handler)calc_negate, 3,0);
        }else{
            instance = operator_create((operator_handler)calc_sub, 1,0);
        }
    }else if (op=='*'){
        instance = operator_create((operator_handler)calc_mult, 3,0);
    }else if (op=='/'){
        instance = operator_create((operator_handler)calc_div, 3,0);
    }else if (op=='%'){
        instance = operator_create((operator_handler)calc_mod, 2,0);
    }else if (op=='^'){
        instance = operator_create((operator_handler)calc_pow, 4,0);
    }else if (op=='!'){
        instance = operator_create((operator_handler)calc_fact, 4,0);
    }else if (op=='('){
        instance = operator_create(NULL, 0,1);
    }else if (op==')'){
        instance = operator_create(NULL, 0,0);
    }

    return instance;
}

l_operator* operator_create(operator_handler handler, const char precedence, const char is_open_bracket){
    l_operator* instance;

    instance = (l_operator*)malloc(sizeof(l_operator));

    if (!instance){
        return NULL;
    }

    instance->handler = handler;
    instance->pecedence = precedence;
    instance->is_open_bracket = is_open_bracket;

    return instance;
}

void operator_destroy(l_operator** op){
    if (!op || !*op){
        return;
    }

    free(*op);
    *op = NULL;
}

calculator* calc_create(){
    calculator* instance;

    instance = (calculator*)malloc(sizeof(calculator));

    if (!instance){
        return NULL;
    }

    instance->operators = vector_create(sizeof(l_operator*),  (vec_it_dealloc_type)operator_destroy);

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

operator_handler get_operator_handler (const char c){
    if (c == '+'){
        return (operator_handler)calc_sum;
    }

    return NULL;
}

bigint* calc_sum(vector* values, int* error){
    bigint** first, **second, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

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

bigint* calc_sub(vector* values, int* error){
    bigint** first, **second, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_sub(*first,*second);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_mult(vector* values, int* error){
    bigint** first, **second, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_mult(*first,*second);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_div(vector* values, int* error){
    bigint** first, **second, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_div(*first,*second,NULL);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_mod(vector* values, int* error){
    bigint** first, **second, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_mod(*first,*second);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_pow(vector* values, int* error){
    bigint** first, **second, *result;

    if (!values || !error){
        return NULL;
    }

    *error = 0;

    second = (bigint**)vector_pop_last(values);
    first = (bigint**)vector_pop_last(values);

    if (!first || !second){
        return NULL;
    }

    result = l_pow(*first,*second, error);
    bigint_destroy(first);
    bigint_destroy(second);

    return result;
}

bigint* calc_fact(vector* values, int* error){
    bigint** first, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    first = (bigint**)vector_pop_last(values);

    if (!first){
        return NULL;
    }

    result = l_fact(*first, error);
    bigint_destroy(first);

    return result;
}

bigint* calc_negate(vector* values, int* error){
    bigint** first, *result;
    if (!values || !error){
        return NULL;
    }

    *error = 0;

    first = (bigint**)vector_pop_last(values);

    if (!first){
        return NULL;
    }

    result = bigint_negate(*first);
    bigint_destroy(first);

    return result;
}

int add_operator(calculator* calc, l_operator* operator, int* error){
    l_operator* last_op;
    bigint* result;
    if (!calc || !operator || !error){
        return 0;
    }

    *error = 0;

    if (vector_isempty(calc->operators)){
        vector_push_back(calc->operators, &operator);
        return 1;
    }

    while (!vector_isempty(calc->operators)){   
        if (operator->is_open_bracket){
            break;
        } 
        last_op = *((l_operator**)vector_peek_last(calc->operators));
        if (last_op->pecedence >= operator->pecedence){
            if (!last_op->handler){
                if (!operator->handler){
                    if (!operator->is_open_bracket){
                        last_op = *((l_operator**)vector_pop_last(calc->operators));
                        operator_destroy(&operator);
                        operator_destroy(&last_op);
                        return 1;
                    }else{
                        break;
                    }
                }
            }
            last_op = *((l_operator**)vector_pop_last(calc->operators));
            if (!(result = last_op->handler(calc->values, error))){
                operator_destroy(&last_op);
                operator_destroy(&operator);
                return 0;
            }
            operator_destroy(&last_op);
            if (!vector_push_back(calc->values,&result)){
                operator_destroy(&operator);
                return 0;
            }
        }else{
            break;
        }
    }

    return vector_push_back(calc->operators, &operator);
}

bigint* calc_calculate(calculator* calc ,const vector* tokens, int* error){
    size_t len,i;
    cstring* token;
    bigint* value;
    l_operator* operator;
    char c, op_prev = 1;

    if (!tokens || !calc || !error){
        return NULL;
    }

    *error = 0;

    len = vector_count(tokens);

    for (i=0;i<len;++i){
        token = *(cstring**)vector_at(tokens,i);
        if (string_size(token)==1){
            c = string_char_at(token,0);
            if (c<'0' || c>'9'){
                if (op_prev)
                    operator = operator_init(c, 1);
                else{
                    operator = operator_init(c, 0);
                }
                if (!operator){
                    return NULL;
                }
                if (!add_operator(calc,operator, error)){
                    return NULL;
                }
                op_prev = 1;
                continue;
            }
        }
        op_prev =0;
        value = get_bigint(token);
        if (!value){
            return NULL;
        }
        if (!vector_push_back(calc->values, &value)){
            return NULL;
        }
    }

    while (!vector_isempty(calc->operators)){
        operator =  *((l_operator**)vector_pop_last(calc->operators));
        if (!operator->handler){
            *error = SYNTAX_ERROR;
            operator_destroy(&operator);
            return NULL;
        }

        value = operator->handler(calc->values, error);
        operator_destroy(&operator);
        if (!value){
            if (*error==0){
                *error = SYNTAX_ERROR;
            }
            return NULL;
        }
        if (!vector_push_back(calc->values, &value)){
            return NULL;
        }
    }

    if (vector_count(calc->values)!=1){
        *error = SYNTAX_ERROR;
        return NULL;
    }
    return *((bigint**)vector_pop_last(calc->values));
}