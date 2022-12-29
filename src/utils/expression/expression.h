#ifndef EXPRESSION
#define EXPRESSION

#include <stdlib.h>
#include "../cstring/cstring.h"
#include "../vector/vector.h"

vector* expression_parse(cstring* string, char* operands);

int expression_validate(vector* tokens, char* operands);

int validate_number (cstring* number);

int contains (const char* characters, char c);

#endif