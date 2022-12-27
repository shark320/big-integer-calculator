#ifndef STRING
#define STRING

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../vector/vector.h"

typedef struct {
    vector* chars;
} cstring;

cstring* string_create ();

cstring* string_init(const char* value);

void string_destroy(cstring** string);

char* get_chars(const cstring* string);

int string_reverse(cstring** string);

size_t string_size(const cstring* string);

int string_append (cstring* string, const char value);

char string_char_at(const cstring* string, size_t index);

void string_destroy_t(void** string);

/* int string_contains_char(const cstring* string, const char c); */

#endif