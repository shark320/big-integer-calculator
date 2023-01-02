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

    #undef EXIT_IF_NOT

    return tokens;
}

int validate_bin (const cstring* number){
    size_t len,i;
    char c;

    if (!number){
        return 0;
    }

    len = string_size(number);

    if (len<3){
        return 0;
    }

    if (string_char_at(number,0)!='0'){
        return 0;
    }

    if (string_char_at(number,1)!='b'){
        return 0;
    }

    for (i=2;i<len;++i){
        c=string_char_at(number,i);
        if ( !(c=='1' || c=='0')){
            return 0;
        }
    }

    return 1;
}

int validate_hex (const cstring* number){
    size_t len,i;
    char c;

    if (!number){
        return 0;
    }

    len = string_size(number);

    if (len<3){
        return 0;
    }

    if (string_char_at(number,0)!='0'){
        return 0;
    }

    if (string_char_at(number,1)!='x'){
        return 0;
    }

    for (i=2;i<len;++i){
        c=string_char_at(number,i);
        if ( ! ((c>='0' && c<='9') || (c>='a' && c<='f'))){
            return 0;
        }
    }

    return 1;
}

int validate_dec (const cstring* number){
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

int validate_number (const cstring* number){
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

char simple_pow (const char base, const char pow){
    int res=1,i;
    for (i=0;i<pow;++i){
        res *= base;
        if (res>255){
            return 0;
        }
    }
    return res;
}

char simple_bin_from_string (const cstring* number){
    size_t len,i;
    char num=0,c;
    if (!number){
        return 0;
    }

    len = string_size(number);

    if (len!=8){
        return 0;
    }

    for (i=0;i<len;++i){
        c=string_char_at(number,i);
        if (c=='1'){
            num+=simple_pow(2,i);
        }
    }

    return num;
}

vector* bin_from_string (const cstring* number){
    vector* bin_number;
    size_t len,i, count=0,j;
    cstring* tmp;
    char d,num;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            vector_destroy(&bin_number); \
            string_destroy(&tmp);\
            return NULL;                     \
        }

    if (!number || !validate_bin(number)){
        return NULL;
    }

    bin_number = vector_create(sizeof(char), NULL);

    if (!bin_number){
        return NULL;
    }

    tmp = string_create();
    if (!tmp){
        vector_destroy(&bin_number);
        return NULL;
    }

    len = string_size(number);

    for (i=len;i>2;--i){
        d = string_char_at(number,i-1);
        string_append(tmp,d);
        ++count;
        if (count==8){
            num = simple_bin_from_string(tmp);
            EXIT_IF_NOT(vector_push_back(bin_number,&num));
            EXIT_IF_NOT(string_clear(tmp));
            count = 0;
            continue;
        }
        if (i==3){
            if (d=='1'){
                for (j=count;j<8;++j){
                    string_append(tmp,'1');
                }
            }else{
                for (j=count;j<8;++j){
                    string_append(tmp,'0');
                }
            }
            num = simple_bin_from_string(tmp);
            EXIT_IF_NOT(vector_push_back(bin_number,&num));
            break;
        }
    }

/*     EXIT_IF_NOT(vector_reverse(bin_number)); */

    string_destroy(&tmp);

    #undef EXIT_IF_NOT

    return bin_number;
}


bigint* bigint_from_bin (const cstring* number){
    bigint* converted;
    vector* bin_number;
    signed char sign = POSITIVE;
    
    if (!number || !validate_bin(number)){
        return NULL;
    }

    bin_number = bin_from_string(number);

    if (!bin_number){
        return NULL;
    }

    if (string_char_at(number,2)=='1'){
        invert_bits(bin_number);
        add_one (bin_number);
        sign = NEGATIVE;
    }

/*     if (!vector_reverse(bin_number)){
        vector_destroy(&bin_number);
        return NULL;
    } */

    converted = bigint_init_vector(bin_number,sign);
    vector_destroy(&bin_number);
    return converted;
}

int invert_bits (vector* number){
    size_t len,i;
    char d;
    if (!number){
        return 0;
    }

    len = vector_count(number);

    for (i=0;i<len;++i){
        d = *((char*)vector_at(number,i));
        *((char*)vector_at(number,i)) = ~d;
    }

    return 1;
}

int add_one (vector* number){
    unsigned char remainder = 0,d,one = 1;
    size_t i,len;

    if (!number){
        return 0;
    }

    len = vector_count(number);

    for (i=0;i<len;++i){
        d = *((char*)vector_at(number,i));
        if (d==255){
            remainder=1;
        }else{
            *((char*)vector_at(number,i)) = d+1;
            remainder=0;
            break;
        }
    }

    if (remainder==1){
        vector_push_back(number,&one);
    }

    return 1;
}

char simple_hex_from_string(cstring* number){
    size_t len,i;
    unsigned char num=0,c,d;
    if (!number){
        return 0;
    }

    len = string_size(number);

    if (len!=2){
        return 0;
    }

    for (i=0;i<len;++i){
        c=string_char_at(number,i);
        if (c>='0' && c<='9'){
            d = c-'0';
        }else{
            d = c-'a' + 10;
        }
        num += d * simple_pow(16,i);
    }

    return num;
}

vector* hex_from_string (const cstring* number){
    vector* hex_number;
    size_t len,i, count=0,j;
    cstring* tmp;
    char d,num;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            vector_destroy(&hex_number); \
            string_destroy(&tmp);\
            return NULL;                     \
        }

    if (!number || !validate_hex(number)){
        return NULL;
    }

    hex_number = vector_create(sizeof(char), NULL);

    if (!hex_number){
        return NULL;
    }

    tmp = string_create();
    if (!tmp){
        vector_destroy(&hex_number);
        return NULL;
    }

    len = string_size(number);

    for (i=len;i>2;--i){
        d = string_char_at(number,i-1);
        string_append(tmp,d);
        ++count;
        if (count==2){
            num = simple_hex_from_string(tmp);
            EXIT_IF_NOT(vector_push_back(hex_number,&num));
            EXIT_IF_NOT(string_clear(tmp));
            count = 0;
            continue;
        }
        if (i==3){
            if (d>'8'){
                for (j=count;j<2;++j){
                    string_append(tmp,'f');
                }
            }else{
                for (j=count;j<2;++j){
                    string_append(tmp,'0');
                }
            }
            num = simple_hex_from_string(tmp);
            EXIT_IF_NOT(vector_push_back(hex_number,&num));
            break;
        }
    }

/*     EXIT_IF_NOT(vector_reverse(hex_number)); */

    string_destroy(&tmp);

    #undef EXIT_IF_NOT

    return hex_number;
}

bigint* bigint_from_hex (const cstring* number){
    bigint* converted;
    vector* hex_number;
    signed char sign = POSITIVE;
    
    if (!number || !validate_hex(number)){
        return NULL;
    }

    hex_number = hex_from_string(number);

    if (!hex_number){
        return NULL;
    }

    if (string_char_at(number,2)>'8'){
        invert_bits(hex_number);
        add_one (hex_number);
        sign = NEGATIVE;
    }

    converted = bigint_init_vector(hex_number,sign);
    vector_destroy(&hex_number);
    return converted;
}

bigint* get_bigint (const cstring* number){

    if(!number){
        return NULL;
    }

    if (validate_dec(number)){
        return bigint_init(number);
    }

    if (validate_hex(number)){
        return bigint_from_hex(number);
    }

    if (validate_bin(number)){
        return bigint_from_bin(number);
    }

    return NULL;
}

int print_dec(const bigint* number){
    cstring* string;

    if (!number){
        return 0;
    }

    string = bigint_to_string(number);

    if (!string){
        return 0;
    }

    printf("%s\n", get_chars(string));

    string_destroy(&string);

    return 1;
}

int print_hex(const bigint* number){
    vector* bytes;
    unsigned char d;
    size_t i;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            vector_destroy(&bytes); \
            return 0;                     \
        }

    if (!number){
        return 0;
    }

    bytes = bigint_to_bytes(number);

    if (!bytes){
        return 0;
    }

    if (number->sign == NEGATIVE){
        EXIT_IF_NOT(invert_bits(bytes));
        EXIT_IF_NOT(add_one(bytes));
        printf("0x");
    }else{
        printf("0x0");
    }
    

    for(i=vector_count(bytes);i>0;--i){
        d = *((char*)vector_at(bytes,i-1));
        if (number->sign == NEGATIVE && i==vector_count(bytes) && d>=0xf0){
            d -= 0xf0;
        }
        printf("%x",d);
    }

    printf("\n");


    vector_destroy(&bytes);

    #undef EXIT_IF_NOT
    return 1;
}

int print_bin(const bigint* number){
    vector* bytes;
    unsigned char d;
    unsigned char bit;
    size_t i,j;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            vector_destroy(&bytes); \
            return 0;                     \
        }

    if (!number){
        return 0;
    }

    bytes = bigint_to_bytes(number);

    if (!bytes){
        return 0;
    }

    if (number->sign == NEGATIVE){
        EXIT_IF_NOT(invert_bits(bytes));
        EXIT_IF_NOT(add_one(bytes));
        printf("0b");
    }else{
        printf("0b0");
    }
    

    for(i=vector_count(bytes);i>0;--i){
        d = *((char*)vector_at(bytes,i-1));
        for (j=8;j>0;--j){
            bit =  (d>>(j-1))&1;
            if (i==vector_count(bytes)){
                if ((number->sign == POSITIVE && bit==0) || (number->sign == NEGATIVE && bit==1 && j!=1)){
                    continue;
                }
            }
            printf("%d",bit);
        }
    }

    printf("\n");


    vector_destroy(&bytes);

    #undef EXIT_IF_NOT
    return 1;
}