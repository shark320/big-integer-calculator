#ifndef EXPRESSION
#define EXPRESSION

#include <stdlib.h>
#include "../cstring/cstring.h"
#include "../vector/vector.h"

vector* expression_parse(cstring* string, char* operands);

#endif