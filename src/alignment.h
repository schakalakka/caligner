//
// alignment.h
//
// Created by andreas on 1/13/17.
//

#ifndef ALIGNER_ALIGNMENT_H
#define ALIGNER_ALIGNMENT_H

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "minmax.h"
#include <stdbool.h>

extern bool VERBOSE;

typedef struct {
    int match;
    int mismatch;
    int gap_open;
    int gap_extend;
    bool top;
    bool left;
    bool right;
    bool bottom;
    int lower_diagonal;
    int upper_diagonal;
    bool local;
} alignconfig;

typedef alignconfig* alignconfig_t;

alignconfig_t create_default_alignconfig();

void destroy_alignconfig(alignconfig_t);

void
traceback(const char* a, unsigned int len_a, const char* b, unsigned int len_b, unsigned int trace[len_a+1][len_b+1][3],
        unsigned int result_row,
        unsigned int result_col, unsigned int result_spec);

int align(const char*, unsigned int, const char*, unsigned int, const alignconfig_t);

#endif //ALIGNER_ALIGNMENT_H
