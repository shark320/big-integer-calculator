
#ifndef FUTILS
#define FUTILS

#include <stdio.h>
#include <string.h>
#include "../vector/vector.h"

#define BUFFER_SIZE 2000

vector* file_lines(const char* filename);

#endif