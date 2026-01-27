#pragma once

#include <stdlib.h>
#include <stdio.h>

#define throw_error(message) do { \
    fprintf(stderr, "%s\n", message); \
    exit(EXIT_FAILURE); \
} while(0)
