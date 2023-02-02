
#include "long_arithmetic.h"

bigint* bigint_create (){
    bigint* new_number;
/*     int zero = 0; */

    new_number = (bigint*)malloc(sizeof(bigint));

    if (!new_number){
        return NULL;
    }

    new_number->digits = vector_create(sizeof(char),  NULL);

    if (!new_number->digits){
        free(new_number);
        return NULL;
    }

/*     vector_push_back(new_number->digits, &zero); */
    new_number->sign = 1;
    return new_number;
}

bigint* bigint_init_chars (const char* value){
    size_t i;
    size_t len;
    char c, num;
    bigint* new_number;

      #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&new_number);   \
            return NULL;                     \
        }

    if (!value){
        return NULL;
    }

    len = strlen(value);

    if (len==0){
        return NULL;
    }

    new_number = bigint_create();
    if (!new_number){
        return NULL;
    }

    i=0;
    c = value[i];
   

    if (c=='-'){
        new_number->sign = NEGATIVE;
        ++i;
    }else if (c=='+'){
        new_number->sign = POSITIVE;
        ++i;
    }


    for (;i<len;++i){
        c = value[i];
        num = c - '0';
        EXIT_IF_NOT(vector_push_back(new_number->digits,&num));
    }

    EXIT_IF_NOT(vector_reverse(new_number->digits));

    EXIT_IF_NOT(bigint_trim(new_number));

    if (bigint_is_zero(new_number)){
        new_number->sign = POSITIVE;
    }

    #undef EXIT_IF_NOT

    return new_number;
}


bigint* bigint_init (const cstring* value){
    bigint* new_number;

    new_number = bigint_init_chars(get_chars(value));

    return new_number;
}

bigint* bigint_init_long (long value){
    bigint* new_number;
    char d;

    new_number = bigint_create();
    if (!new_number){
        return NULL;
    }

    if (value<0){
        new_number->sign = NEGATIVE;
    }else{
        new_number->sign = POSITIVE;
    }

    value = abs(value);

    do{
        d = value % 10;
        value /= 10;
        if (!vector_push_back(new_number->digits, &d)){
            bigint_destroy(&new_number);
            return NULL;
        }
    }while(value != 0);

    if (bigint_is_zero(new_number)){
        new_number->sign = POSITIVE;
    }

    return new_number;
}

bigint* bigint_init_vector(vector* value, signed char sign){
    bigint* new_number, *pow, *base_num, *tmp;
    size_t len,i;
    int out,error;
    unsigned char num;


      #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&new_number);   \
            bigint_destroy(&base_num);   \
            return NULL;                     \
        }

    
    if (!value){
        return NULL;
    }

    new_number = bigint_zero();

    if (!new_number){
        return NULL;
    }

    base_num = bigint_init_long(256);

    if (!base_num){
        bigint_destroy(&new_number);
        return NULL;
    }

    len = vector_count(value);

    for (i=0;i<len;++i){
        num = *((char*)vector_at(value,i));
        EXIT_IF_NOT(pow = l_pow_long(base_num,i,&error));
        if (! (tmp = l_mult_long(pow,num))){
            bigint_destroy(&pow);
            EXIT_IF_NOT(0);
        }
        out = l_sum_assign(&new_number,tmp);
        bigint_destroy(&pow);
        bigint_destroy(&tmp);
        EXIT_IF_NOT(out);
    }

    bigint_destroy(&base_num);
    new_number->sign = sign;

    #undef EXIT_IF_NOT
    return new_number;
}

long bigint_to_long (const bigint* value){
    long long_value = 0;
    size_t len,i;
    signed char d;

    if (!value){
        return 0;
    }

    len = bigint_size(value);

    for (i = len;i>0;--i){
        d = bigint_digit_at(value,i-1);
        if (d==-1){
            return 0;
        }
        long_value *=10;
        long_value += d;
    }

    if (value->sign == NEGATIVE){
        long_value = -long_value;
    }

    return long_value;
}

vector* bigint_to_bytes (const bigint* value){
    vector* bytes;
    bigint* mod=NULL, *copy,*base;
    char n;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&mod);   \
            bigint_destroy(&base);   \
            bigint_destroy(&copy);   \
            vector_destroy(&bytes);\
            return NULL;                     \
        }

    if (!value){
        return NULL;
    }

    base = bigint_init_long(256);

    if (!base){
        return NULL;
    }

    bytes = vector_create(sizeof(char),NULL);

    if (!bytes){
        bigint_destroy(&base);
        return NULL;
    }

    copy = bigint_copy_abs(value);
    if (!copy){
        bigint_destroy(&base);
        vector_destroy(&bytes);
        return NULL;
    }

    do{
/*         mod = l_mod(copy,base);
        if (!mod){
            bigint_destroy(&base);
            vector_destroy(&bytes);
            bigint_destroy(&copy);
            return NULL;
        } */
        EXIT_IF_NOT(l_div_assign(&copy,base,&mod));
         if (!mod){
            bigint_destroy(&base);
            vector_destroy(&bytes);
            bigint_destroy(&copy);
            return NULL;
        }
        n = (char)bigint_to_long(mod);
        EXIT_IF_NOT(vector_push_back(bytes,&n));
        bigint_destroy(&mod);
    }while(!bigint_is_zero(copy));

    bigint_destroy(&base);  
    bigint_destroy(&copy);   

    #undef EXIT_IF_NOT

    return bytes;
}

bigint* bigint_zero(){
    bigint* zero_number;

    zero_number = bigint_init_chars("0");

    return zero_number;
}

int bigint_trim (const bigint* value){
    size_t len,i;
    signed char d;
    if (!value){
        return 0;
    }

    len = bigint_size(value);

    for (i=len-1;i>0;--i){
        if ((d=bigint_digit_at(value,i))==-1){
            return 0;
        }
        if (d!=0){
            break;
        }
    }

    return vector_resize(value->digits,i+1);
}

size_t bigint_size (const bigint* value){
    if (!value){
        return -1;
    }

    return vector_count (value->digits);
}

cstring* bigint_to_string(const bigint* value){
    int i;
    size_t len;
    cstring* string;
    char c,minus='-';
    signed char d;
    if (!value){
        return NULL;
    }

    string = string_create();

    if (!string){
        return NULL;
    }

    if (value->sign == NEGATIVE){
        string_append(string,minus);
    }

    len = bigint_size(value);

    for (i=len-1;i>=0;--i){
        if((d = bigint_digit_at(value,i))==-1){
            string_destroy(&string);
            return NULL;
        }
        c = d + '0';
        string_append(string, c);
    }

    return string;
}

void bigint_destroy (bigint** value){
    if (!value || !*value){
        return;
    }

    vector_destroy(&(*value)->digits);
    free(*value);
    *value = NULL;
}

bigint* bigint_copy (const bigint* to_copy){
    bigint* copy;

    if  (!to_copy){
        return NULL;
    }

    copy = bigint_create();

    if (!copy){
        return NULL;
    }

    copy->sign = to_copy->sign;
    vector_destroy(&copy->digits);
    copy->digits = vector_copy(to_copy->digits);

    if (!copy->digits){
        bigint_destroy(&copy);
        return NULL;
    }

    return copy;
}

bigint* bigint_copy_abs (const bigint* value){
    bigint* copy;

    copy = bigint_copy(value);
    if (!copy){
        return NULL;
    }

    copy->sign = POSITIVE;
    return copy;
}

signed char bigint_digit_at (const bigint* value, size_t index){

    if (!value || index >= bigint_size(value)){
        return -1;
    }

    return *((char*)vector_at(value->digits, index));
}

int fix_overflow(bigint* value){
    size_t i,len;
    signed char carry,d;
    if (!value){
        return 0;
    }

    len = bigint_size(value);


    for(i=0;i<len;++i){
        if ((d = bigint_digit_at(value,i))==-1){
            return 0;
        }
        if (d>9){
            *((char*)vector_at(value->digits,i))=d%10;
            carry = d/10;
            if (i==len-1){
                vector_push_back(value->digits,&carry);
            }else{
                *((char*)vector_at(value->digits,i+1))+=carry;
            }
        }
    }


    return 1;
}

int bigint_is_zero(const bigint* value){
    if (!value){
        return 0;
    }

    if (bigint_size(value) == 1 && bigint_digit_at(value,0)==0){
        return 1;
    }

    return 0;
}

int bigint_is_one (const bigint* value){
    if (!value){
        return 0;
    }

    if (bigint_size(value) == 1 && bigint_digit_at(value,0)==1){
        return 1;
    }

    return 0;
}

int l_inc(bigint* value){
    signed char d,sum;

    if (!value){
        return 0;
    }


        d = bigint_digit_at(value,0);
        if (d==-1){
            return 0;
        }
        sum = d+1;
        if (!vector_push(value->digits, &sum,0)){
            return 0;
        }
    

    return fix_overflow(value);
}

int add_smaller_bignint (bigint* first, const bigint* second){
    size_t len1,len2,i;
    signed char d1,d2,sum;

    if (!first || !second){
        return 0;
    }

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    if (len1<len2){
        return 0;
    }

    for(i=0;i<len2;++i){
        d1 = bigint_digit_at(first,i);
        d2 = bigint_digit_at(second,i);
        if (d1==-1 || d2==-1){
            return 0;
        }
        sum = d1+d2;
        if (!vector_push(first->digits, &sum,i)){
            return 0;
        }
    }

    return fix_overflow(first);
}

bigint* l_sum_abs (const bigint*  first, const bigint* second){
    bigint* result;
    size_t len1,len2;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&result);   \
            return NULL;                     \
        }

    if (!first || !second){
        return NULL;
    }

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    if (len1 >= len2){
        result = bigint_copy_abs(first);
        if (!result){
            return NULL;
        }
        EXIT_IF_NOT(add_smaller_bignint(result,second));
    }else{
        result = bigint_copy_abs(second);
        if (!result){
            return NULL;
        }
        EXIT_IF_NOT(add_smaller_bignint(result,first));
    }

    #undef EXIT_IF_NOT

    return result;
}

bigint* l_sum (const bigint*  first, const bigint* second){
    bigint* result;

    if (!first || !second){
        return NULL;
    }

    if (first->sign == POSITIVE){
        if (second->sign == POSITIVE){
            return l_sum_abs(first,second);
        }else{
            return l_sub_abs(first,second);
        }
    }else{
        if (second->sign == POSITIVE){
            return l_sub_abs(second, first);
        }else{
            result = l_sum_abs(first, second);
            if (!result){
                return NULL;
            }
            result->sign = NEGATIVE;
            return result;
        }
    }
}

int l_sum_assign (bigint**  first, const bigint* second){
     bigint* result;
    if (!first || !*first || !second){
        return 0;
    }

    result = l_sum(*first, second);

    if (!result){
        return 0;
    }

    bigint_destroy(first);

    *first = result;

    return 1;
}

int bigint_cmp_abs (const bigint* first, const bigint* second){
    size_t len1,len2;
    int i;
    signed char d1,d2;
    if(!first || !second){
        return INT_MIN;
    }

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    if (len1 < len2){
        return LESS;
    }

    if (len1 > len2){
        return GREATER;
    }

    for (i=len1-1;i>=0;--i){
        d1 = bigint_digit_at(first, i);
        d2 = bigint_digit_at(second, i);

        if (d1==-1 || d2==-1){
            return INT_MIN;
        }

        if (d1<d2){
            return LESS;
        }

        if (d1>d2){
            return GREATER;
        }
    }

    return EQUALS;
}

int bigint_cmp (const bigint* first, const bigint* second){
    if(!first || !second){
        return INT_MIN;
    }

    if (first->sign < second->sign){
        return LESS;
    }

    if (first->sign > second->sign){
        return GREATER;
    }

    return bigint_cmp_abs(first,second);
}

int fix_borrow (bigint* value){
    size_t i,len;
    signed char borrow,d;
    if (!value){
        return 0;
    }

    len = bigint_size(value);

    for(i=0;i<len;++i){
        d = bigint_digit_at(value,i);
        if (d<0){
            if (i==len-1){
                return 0;
            }
            borrow = abs(d+1)/10 + 1;
            *((char*)vector_at(value->digits,i))+=borrow*10;
            *((char*)vector_at(value->digits,i+1))-=borrow;
        }
    }

    return 1;
}

int l_sub_smaller (bigint*  first, const bigint*  second){
    int cmp;
    size_t len,i;
    signed char d1,d2, sub;

    if (!first || !second){
        return 0;
    }

    cmp = bigint_cmp(first,second);

    if (cmp == LESS){
        return 0;
    }

    len = bigint_size(second);

    for (i=0;i<len;++i){
        d1 = bigint_digit_at(first, i);
        d2 = bigint_digit_at(second,i);
        if(d1 == -1 || d2 == -1){
            return 0;
        }
        sub = d1-d2;
        if (!vector_push(first->digits,&sub,i)){
            return 0;
        }
    }

    return fix_borrow(first);

}

bigint* l_sub_abs (const bigint*  first, const bigint*  second){
    bigint* result;
    int cmp;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&result);   \
            return NULL;                     \
        }


    if (!first || !second){
        return NULL;
    }

    cmp = bigint_cmp_abs(first,second);

    if (cmp == EQUALS){
        return bigint_zero();
    }

    if (cmp == GREATER){
        result = bigint_copy_abs(first);
        if (!result){
            return NULL;
        }
        EXIT_IF_NOT(l_sub_smaller(result, second));
    }else{
        result = bigint_copy_abs(second);
        if (!result){
            return NULL;
        }
        EXIT_IF_NOT(l_sub_smaller(result, first));
        result->sign = NEGATIVE;
    }


    EXIT_IF_NOT(bigint_trim(result));

    #undef EXIT_IF_NOT

    return result;
}

bigint* l_sub (const bigint*  first, const bigint*  second){
    bigint* result;

    if (!first || !second){
        return NULL;
    }

    if (first->sign == POSITIVE){
        if (second->sign == POSITIVE){
            return l_sub_abs(first,second);
        }else{
            return l_sum_abs(first,second);
        }
    }else{
        if (second->sign == POSITIVE){
              result = l_sum_abs(first, second);
            if (!result){
                return NULL;
            }
            result->sign = NEGATIVE;
            return result;
           
        }else{
           return l_sub_abs(second, first);
        }
    }
}

int l_sub_assign (bigint**  first, const bigint*  second){
    bigint* result;
    if (!first || !*first || !second){
        return 0;
    }

    result = l_sub(*first, second);

    if (!result){
        return 0;
    }

    bigint_destroy(first);

    *first = result;

    return 1;
}

int l_sub_assign_abs (bigint**  first, const bigint*  second){
    bigint* result;
    if (!first || !*first || !second){
        return 0;
    }

    result = l_sub_abs(*first, second);

    if (!result){
        return 0;
    }

    bigint_destroy(first);

    *first = result;

    return 1;
}

bigint* l_mult_abs (const bigint*  first, const bigint*  second){
    size_t len1, len2, i,j;
    signed char d,d1,d2,mult,sum;
    bigint* result;

    if (!first || !second){
        return 0;
    }

    if (bigint_is_zero(second)){
        return bigint_zero();
    }

    if (bigint_is_one(second)){
        return bigint_copy(first);
    }

    result = bigint_zero();

    if (!result){
        return 0;
    }
    

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    for (j=0;j<len2;++j){
        for (i=0; i<len1;++i){
            d1 = bigint_digit_at(first, i);
            d2 = bigint_digit_at(second, j);
            if (d1==-1 || d2 == -1){
                return 0;
            }
            mult=d1*d2;
            if ( (i+j) == vector_count(result->digits)){
                vector_push_back(result->digits,&mult);
            }else{
                d = bigint_digit_at(result, i+j);
                if (d==-1){
                    return 0;
                }
                sum = d + mult;
                vector_push(result->digits, &sum, i+j);
               
            }
        }
        if (!fix_overflow(result)){
            return 0;
        }
    }

    if (!bigint_trim(result)){
        bigint_destroy(&result);
        return NULL;
    }

    return result;
}

bigint* l_mult (const bigint*  first, const bigint*  second){
    bigint* result;

    if (!first || !second){
        return NULL;
    }

    result = l_mult_abs (first, second);

    if (!result){
        return NULL;
    }

    if (first->sign == second->sign){
        result->sign = POSITIVE;
    }else{
        result->sign = NEGATIVE;
    }

    return result;
}

int l_mult_assign (bigint** first, const bigint*  second){
    bigint* result;
    if (!first || !*first || !second){
        return 0;
    }

    result = l_mult(*first, second);

    if (!result){
        return 0;
    }

    bigint_destroy(first);

    *first = result;

    return 1;
}

bigint* l_mult_long (const bigint*  first, const long second){
    bigint *result, *long_number;

    long_number = bigint_init_long (second);

    result =  l_mult(first,long_number);
    bigint_destroy(&long_number);

    return result;
}

bigint* bigint_get_n_first_digits (const bigint* value, size_t n){
    bigint* new_number;
    size_t len,i;
    signed char d;

    #define EXIT_IF(expression)     \
        if ((expression)) {            \
            bigint_destroy(&new_number);   \
            return NULL;                     \
        }

    if (!value){
        return NULL;
    }

    new_number = bigint_create();
    if (!new_number){
        return NULL;
    }

    len = bigint_size(value);


    EXIT_IF (len<n);

    for (i=len;(len-i)<n;--i){
        d = bigint_digit_at(value,i-1);
        EXIT_IF(d==-1);
        EXIT_IF(!vector_push_back(new_number->digits,&d));
    }

    EXIT_IF(!vector_reverse(new_number->digits));

    #undef EXIT_IF
    return new_number;
}

int l_div_simple (const bigint* first, const bigint* second){
    bigint* approx;
    int count = 0, cmp;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            bigint_destroy(&approx);   \
            return -1;                     \
        }

    if (!first || !second){
        return -1;
    }

    approx = bigint_zero();

    if (!approx){
        return -1;
    }

    for(;;){
        ++count;
        EXIT_IF_NOT(l_sum_assign(&approx, second));
        cmp = bigint_cmp_abs(first,approx);
        if (cmp == EQUALS){
            break;
        }
        if (cmp == LESS){
            --count;
            break;
        }
    }

    bigint_destroy(&approx);

    #undef EXIT_IF_NOT
    return count;
}

bigint* l_div_abs (const bigint* first, const bigint* second, bigint** mod_abs){
    bigint* result, *tmp,*approx;
    size_t len1,i;
    int cmp, div_simple;
    signed char d, zero = 0;

     #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            printf("return at i=%d\n",i); \
            bigint_destroy(&result);   \
            bigint_destroy(&tmp);   \
            return NULL;                     \
        }

    if (!first || !second){
        return NULL;
    }

    cmp = bigint_cmp_abs(first,second);

    if (cmp==EQUALS){
        return bigint_init_chars("1");
    }

    if (cmp==LESS){
        *mod_abs = bigint_copy_abs(first);
        return bigint_zero();
    }

    result = bigint_create();

    if (!result){
        return NULL;
    }

    tmp = bigint_create();

    if (!tmp){
        bigint_destroy(&result);
        return NULL;
    }

    len1 = bigint_size(first);

    for (i=len1;i>0;--i){
        if (i==16382){
            printf("");
        }
        d = bigint_digit_at(first,i-1);
        EXIT_IF_NOT (vector_push_forward(tmp->digits,&d));
        EXIT_IF_NOT (bigint_trim(tmp));
        cmp = bigint_cmp_abs(tmp,second);
        if (cmp==GREATER || cmp==EQUALS){
            div_simple = l_div_simple(tmp,second);
            EXIT_IF_NOT(approx = l_mult_long(second,div_simple));
            EXIT_IF_NOT (vector_push_back(result->digits, &div_simple));
            EXIT_IF_NOT (l_sub_assign_abs(&tmp,approx));
            bigint_destroy(&approx);
        }else{
            EXIT_IF_NOT (vector_push_back(result->digits, &zero));
        }
    }

/*     printf("tmp: %s\n",get_chars(bigint_to_string(tmp)));

    bigint_destroy(&tmp); */

    if(mod_abs){
        *mod_abs = tmp; 
    }else{
        bigint_destroy(&tmp);
    }
    EXIT_IF_NOT(vector_reverse(result->digits));
    EXIT_IF_NOT(bigint_trim(result));

    #undef EXIT_IF_NOT

    return result;
}

bigint* l_div (const bigint*  first, const bigint*  second, bigint** mod_abs){
    bigint* result;

    result = l_div_abs(first, second, mod_abs);

    if (!result){
        return NULL;
    }

    if (first->sign == second->sign){
        result->sign = POSITIVE;
    }else{
        result->sign = NEGATIVE;
    }

    return result;
}

int l_div_assign (bigint** first, const bigint* second,bigint** mod_abs){
    bigint* result;
    if (!first || !*first || !second){
        return 0;
    }

    result = l_div(*first, second,mod_abs);

    if (!result){
        return 0;
    }

    bigint_destroy(first);

    *first = result;

    return 1;
}

bigint* l_mod (const bigint* first, const bigint* second){
    bigint* result, *div, *mult;

    div = l_div(first,second,NULL);

    if (!div){
        return NULL;
    }

    mult = l_mult (div,second);

    bigint_destroy(&div);

    if (!mult){
        return NULL;
    }

    result = l_sub(first,mult);

    bigint_destroy(&mult);

    if (!result){
        return NULL;
    }

    return result;
}

bigint* l_pow (const bigint* first, const bigint* second, int* error){
    bigint* result, *i;

    if (!first || !second || !error){
        return NULL;
    }

    *error = 0;

    if (bigint_is_zero(second)){
        return bigint_init_chars("1");
    }

    if (second->sign == NEGATIVE){
        if (bigint_is_zero(first)){
            *error = ZERO_DIVISION;
            return NULL;
        }
        if (bigint_is_one(first)){
            if (first->sign == POSITIVE){
                return bigint_init_chars("1");
            }else{
                return bigint_init_chars("-1");
            }
        }else{
            return bigint_zero();
        }
    }

    i = bigint_zero();

    if (!i){
        return NULL;
    }

    result = bigint_init_chars("1");


    if (!result){
        bigint_destroy(&i);
        return NULL;
    }



    for (;bigint_cmp(i,second)==LESS;){
        if (!l_mult_assign(&result,first) || !l_inc(i)){
             bigint_destroy(&i);
             bigint_destroy(&result);
            return NULL;
        }
    }

     bigint_destroy(&i);

    return result;
}

bigint* l_pow_long (const bigint* first, const long second, int* error){
    bigint* result, *second_bigint;
    if (!first || !error){
        return NULL;
    }

    second_bigint = bigint_init_long(second);
    if (!second_bigint){
        return NULL;
    }

    result = l_pow(first,second_bigint,error);

    bigint_destroy (&second_bigint);

    return result;
}

bigint* l_fact(const bigint* value, int* error){
     bigint* result, *i;

    if (!value || !error){
        return NULL;
    }

    if (bigint_is_zero(value)){
        return bigint_init_chars("1");
    }

    if (value->sign == NEGATIVE){
        *error = NEGATIVE_FACT;
        return NULL;
    }

    i = bigint_init_chars("2");

    if (!i){
        return NULL;
    }

    result = bigint_init_chars("1");

    if (!result){
        bigint_destroy(&i);
        return NULL;
    }

    for (;bigint_cmp(i,value)!=GREATER;){
        if (!l_mult_assign(&result,i) || !l_inc(i)){
             bigint_destroy(&i);
             bigint_destroy(&result);
            return NULL;
        }
    }

    bigint_destroy(&i);

    return result;
}

bigint* bigint_negate(bigint* value){
    bigint* result;
    if (!value){
        return 0;
    }

    result = bigint_copy(value);
    if (!result){
        return NULL;
    }

    if (bigint_is_zero(value)){
        return result;
    }

    if (value->sign == POSITIVE){
        result->sign = NEGATIVE;
    }else{
        result->sign = POSITIVE;
    }

    return result;
}