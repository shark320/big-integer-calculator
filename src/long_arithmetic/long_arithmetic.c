
#include "long_arithmetic.h"

bigint* bigint_cretate (){
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

bigint* bigint_init_chars (const char* value, const short int sign){
    size_t i;
    size_t len;
    char c, num;
    bigint* new_number;

    if (!value){
        return NULL;
    }

    /* underfined big integer sign */
    if (sign==0){
        return NULL;
    }

    len = strlen(value);
    new_number = bigint_cretate();
    if (!new_number){
        return NULL;
    }

    new_number->sign = sign<0?NEGATIVE:POSITIVE;

    for (i=0;i<len;++i){
        c = value[i];
        num = c - '0';
        vector_push_back(new_number->digits,&num);
    }

    vector_reverse(new_number->digits);

    bigint_trim(new_number);

    return new_number;
}


bigint* bigint_init (const cstring* value, const short int sign){
    bigint* new_number;

    new_number = bigint_init_chars(get_chars(value), sign);

    return new_number;
}

bigint* bigint_zero(){
    bigint* zero_number;

    zero_number = bigint_init_chars("0", POSITIVE);

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

    copy = bigint_cretate();

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
        vector_push(first->digits, &sum,i);
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
        if ((d = bigint_digit_at(value,i))==-1){
            return 0;
        }
        if (d<0){
            if (i==len-1){
                return 0;
            }
            borrow = abs(d/10) + 1;
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

bigint* l_mult_abs (const bigint*  first, const bigint*  second){
    size_t len1, len2, i,j;
    signed char d,d1,d2,mult,sum;
    bigint* result;

    if (!first || !second){
        return 0;
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

    new_number = bigint_cretate();
    if (!new_number){
        return NULL;
    }

    len = bigint_size(value);

    if (n>len){
        return NULL;
    }

    for (i=len;(len-i)<n;--i){
        d = bigint_digit_at(value,i-1);
        EXIT_IF(d==-1);
        EXIT_IF(!vector_push_back(new_number->digits,&d));
    }

    EXIT_IF(!vector_reverse(new_number->digits));

    #undef EXIT_IF
    return new_number;
}