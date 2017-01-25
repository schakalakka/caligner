//
// overlap.pyinterface
//
// Created by andreas on 1/5/17.
//

#include "overlap.h"

int naive_overlap(char* a, unsigned int len_a, char* b, unsigned int len_b)
{
    int max_score = 0;
    char loop_broke = 0;
    for (int k = 1; k<min(len_a, len_b); ++k) {
        for (int i = 1; i<=k; ++i) {
            if (a[len_a-i]!=b[k-i]) {
                loop_broke = 1;
                break;
            }
        }
        if (loop_broke==0) max_score = k;
        else loop_broke = 0;
    }
    return max_score;
}

int overlap(char* a, unsigned int len_a, char* b, unsigned int len_b)
{
    alignconfig_t conf = create_default_alignconfig();
    conf->left = true;
    conf->bottom = true;
    conf->mismatch = -(len_a+len_b+1);
    conf->gap_open = -(len_a+len_b+1);
    conf->gap_extend = -(len_a+len_b+1);

    return align(a, len_a, b, len_b, conf);
}