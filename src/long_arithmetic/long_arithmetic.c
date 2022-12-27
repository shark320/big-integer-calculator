
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

bigint* bigint_init (const cstring* value, const int sign){
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

    len = string_size(value);

    new_number = bigint_cretate();
    if (!new_number){
        return NULL;
    }

    new_number->sign = sign<0?NEGATIVE:POSITIVE;

    for (i=0;i<len;++i){
        c = string_char_at(value,i);
        num = c - '0';
        vector_push_back(new_number->digits,&num);
    }

    vector_reverse(new_number->digits);

    return new_number;
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

signed char bigint_digit_at (const bigint* value, size_t index){

    if (!value || index >= bigint_size(value)){
        return -1;
    }

    return *((char*)vector_at(value->digits, index));
}

void add_smaller_bignint (bigint* first, const bigint* second){
    size_t len1,len2,i;
    signed char d1,d2,sum;

    if (!first || !second){
        return;
    }

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    if (len1<len2){
        return;
    }

    for(i=0;i<len2;++i){
        d1 = bigint_digit_at(first,i);
        d2 = bigint_digit_at(second,i);
        if (d1==-1 || d2==-1){
            return;
        }
        sum = d1+d2;
        vector_push(first->digits, &sum,i);
    }
}

int fix_overflow(bigint* value){
    size_t i,len;
    signed char one = 1,d,sub;
    if (!value){
        return 0;
    }

    len = bigint_size(value);


    for(i=0;i<len;++i){
        if ((d = bigint_digit_at(value,i))==-1){
            return 0;
        }
        if (d>9){
            *((char*)vector_at(value->digits,i))=d-10;
            if (i==len-1){
                vector_push_back(value->digits,&one);
            }else{
                *((char*)vector_at(value->digits,i+1))+=1;
            }
        }
    }


    return 1;
}

bigint* l_sum (const bigint*  first, const bigint* second){
    bigint* result;
    size_t len1,len2;

    if (!first || !second){
        return NULL;
    }

    len1 = bigint_size(first);
    len2 = bigint_size(second);

    if (len1 >= len2){
        result = bigint_copy(first);
        if (!result){
            return NULL;
        }
        add_smaller_bignint(result,second);
    }else{
        result = bigint_copy(second);
        if (!result){
            return NULL;
        }
         add_smaller_bignint(result,first);
    }

    if (!fix_overflow(result)){
        bigint_destroy(&result);
        return NULL;
    }

    return result;
}