#ifndef STRING
#define STRING

#include <string.h>
#include <stdlib.h>
#include "../vector/vector.h"

typedef struct {
    vector* chars;
} cstring;

cstring* string_create ();

cstring* string_init(const char* value);

void string_destroy(cstring** string);

char* get_chars(const cstring* string);

int string_reverse(cstring** string);

size_t string_size(cstring* string);

int string_append (cstring* string, const char value);

char string_char_at(cstring* string, size_t index);

#endif