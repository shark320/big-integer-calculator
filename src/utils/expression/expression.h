#ifndef EXPRESSION
#define EXPRESSION

#include <stdlib.h>
#include "../cstring/cstring.h"
#include "../vector/vector.h"
#include "../../long_arithmetic/long_arithmetic.h"
#include "../errors/errors.h"

#define HEX 1
#define BIN 2
#define DEC 3

vector* expression_parse(cstring* string, char* operands);



int expression_validate(vector* tokens, char* operands);

int validate_number (const cstring* number);

int contains (const char* characters, char c);

bigint* get_bigint (const cstring* number);

bigint* bigint_from_bin (const cstring* number);

bigint* bigint_from_hex (const cstring* number);

vector* bin_from_string (const cstring* number);

vector* hex_from_string (const cstring* number);

int add_one (vector* number);

int invert_bits (vector* number);

int print_dec(const bigint* number);

int print_hex(const bigint* number);

int print_bin(const bigint* number);

int remove_insignificant(cstring** number, int radix,signed char sign);

int is_expression (const cstring* string, char* operands);

#endif