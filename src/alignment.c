//
// alignment.pyinterface
//
// Created by andreas on 1/13/17.
//


#include "alignment.h"

bool VERBOSE = true;

alignconfig_t create_default_alignconfig()
{
    alignconfig_t conf = malloc(sizeof(alignconfig));
    conf->match = 1;
    conf->mismatch = -1;
    conf->gap_open = -1;
    conf->gap_extend = -1;

    conf->top = false;
    conf->left = false;
    conf->right = false;
    conf->bottom = false;

    conf->lower_diagonal = INT_MIN;
    conf->upper_diagonal = INT_MAX;

    conf->local = false;

    return conf;
}

void destroy_alignconfig(alignconfig_t conf)
{
    free(conf);
}

void
traceback(const char* a, unsigned int len_a, const char* b, unsigned int len_b, unsigned int trace[len_a+1][len_b+1][3],
        unsigned int result_row,
        unsigned int result_col, unsigned int result_spec)
{
    char aligned_a[len_a+len_b+1];
    char aligned_b[len_a+len_b+1];
    char middle_string[len_a+len_b+1];
    unsigned int current_index_of_middle_string = len_a+len_b+1;

    memset(aligned_a, '\0', sizeof(aligned_a));
    memset(aligned_b, '\0', sizeof(aligned_b));
    memset(middle_string, '\0', sizeof(middle_string));

    if (result_row<len_a) {
        strncpy(&aligned_a[len_b+result_row], &a[result_row], len_a-result_row);
        memset(&aligned_b[len_b+result_row], '-', len_a-result_row);
        memset(&middle_string[len_b+result_row], ' ', len_a-result_row);
        current_index_of_middle_string = len_b+1+result_row;
    }
    if (result_col<len_b) {
        strncpy(&aligned_b[len_a+result_col], &b[result_col], len_b-result_col);
        memset(&aligned_a[len_a+result_col], '-', len_b-result_col);
        memset(&middle_string[len_a+result_col], ' ', len_b-result_col);
        current_index_of_middle_string = len_a+1+result_col;
    }
    unsigned int new_spec;
    unsigned int nr_gaps_for_a = 0;
    unsigned int nr_gaps_for_b = 0;
    while (result_row>0 || result_col>0) {
        new_spec = trace[result_row][result_col][result_spec];
        current_index_of_middle_string--;
        if (result_spec==0) {
            result_row--;
            result_col--;
            aligned_a[len_b+result_row-nr_gaps_for_a] = a[result_row];
            aligned_b[len_a+result_col-nr_gaps_for_b] = b[result_col];
            if (a[result_row]==b[result_col]) middle_string[current_index_of_middle_string] = '|';
            else middle_string[current_index_of_middle_string] = ' ';

        }
        else if (result_spec==1) {
            result_row--;
            nr_gaps_for_b++;
            aligned_a[len_b+result_row-nr_gaps_for_a] = a[result_row];
            aligned_b[len_a+result_col-nr_gaps_for_b] = '-';
            middle_string[current_index_of_middle_string] = ' ';
        }
        else if (result_spec==2) {
            result_col--;
            nr_gaps_for_a++;
            aligned_a[len_b+result_row-nr_gaps_for_a] = '-';
            aligned_b[len_a+result_col-nr_gaps_for_b] = b[result_col];
            middle_string[current_index_of_middle_string] = ' ';
        }
        result_spec = new_spec;
    }

    printf("\n%s\n%s\n%s\n", &aligned_a[len_b+result_row-nr_gaps_for_a], &middle_string[current_index_of_middle_string],
            &aligned_b[len_a+result_col-nr_gaps_for_b]);
}

int
align(const char* a, unsigned int len_a, const char* b, unsigned int len_b, const alignconfig_t conf)
{
    int current_score;
    int m[len_a+1][len_b+1][3];
    unsigned int trace[len_a+1][len_b+1][3];
    int result_score;
    unsigned int result_row;
    unsigned int result_col;
    unsigned int result_spec;

    m[0][0][0] = 0;
    m[0][0][1] = 0;
    m[0][0][2] = 0;
    trace[0][0][0] = 0;
    trace[0][0][1] = 0;
    trace[0][0][2] = 0;

    for (unsigned int i = 1; i<=len_a; ++i) {
        m[i][0][0] = -(len_a+len_b);
        m[i][0][2] = -(len_a+len_b);
        if (conf->left==true) m[i][0][1] = 0;
        else m[i][0][1] = conf->gap_open+(i-1)*conf->gap_extend;
        trace[i][0][0] = 1;
        trace[i][0][1] = 1;
        trace[i][0][2] = 1;
    }

    for (unsigned int j = 1; j<=len_b; ++j) {
        m[0][j][0] = -(len_a+len_b);
        m[0][j][1] = -(len_a+len_b);
        if (conf->top==true) m[j][0][2] = 0;
        else m[0][j][2] = conf->gap_open+(j-1)*conf->gap_extend;
        trace[0][j][0] = 2;
        trace[0][j][1] = 2;
        trace[0][j][2] = 2;
    }

    for (unsigned int i = 1; i<=len_a; ++i) {
        for (unsigned int j = 1; j<=len_b; ++j) {
            if (a[i-1]==b[j-1]) current_score = conf->match;
            else current_score = conf->mismatch;

            //determine next element of substitution matrix
            if (m[i-1][j-1][0]>=max(m[i-1][j-1][1], m[i-1][j-1][2])) {
                m[i][j][0] = m[i-1][j-1][0]+current_score;
                trace[i][j][0] = 0;
            }
            else if (m[i-1][j-1][1]>=m[i-1][j-1][2]) {
                m[i][j][0] = m[i-1][j-1][1]+current_score;
                trace[i][j][0] = 1;
            }
            else {
                m[i][j][0] = m[i-1][j-1][2]+current_score;
                trace[i][j][0] = 2;
            }

            //determine next element of deletion matrix
            if (m[i-1][j][0]+conf->gap_open>=max(m[i-1][j][1]+conf->gap_extend, m[i-1][j][2]+conf->gap_open)) {
                m[i][j][1] = m[i-1][j][0]+conf->gap_open;
                trace[i][j][1] = 0;
            }
            else if (m[i-1][j][1]+conf->gap_extend>=m[i-1][j][2]+conf->gap_open) {
                m[i][j][1] = m[i-1][j][1]+conf->gap_extend;
                trace[i][j][1] = 1;
            }
            else {
                m[i][j][1] = m[i-1][j][2]+conf->gap_open;
                trace[i][j][1] = 2;
            }

            //determine next element of insertion matrix
            if (m[i][j-1][0]+conf->gap_open>=max(m[i][j-1][1]+conf->gap_open, m[i][j-1][2]+conf->gap_extend)) {
                m[i][j][2] = m[i][j-1][0]+conf->gap_open;
                trace[i][j][2] = 0;
            }
            else if (m[i][j-1][1]+conf->gap_open>=m[i][j-1][2]+conf->gap_extend) {
                m[i][j][2] = m[i][j-1][1]+conf->gap_open;
                trace[i][j][2] = 1;
            }
            else {
                m[i][j][2] = m[i][j-1][2]+conf->gap_extend;
                trace[i][j][2] = 2;
            }
        }
    }

    result_row = len_a;
    result_col = len_b;
    result_score = max(m[len_a][len_b][0], max(m[len_a][len_b][1], m[len_a][len_b][2]));
    // find biggest element in last column if ending gaps for a are free
    if (conf->right==true) {
        for (unsigned int i = 0; i<=len_a; ++i) {
            if (result_score<max(m[i][len_b][0], max(m[i][len_b][1], m[i][len_b][2]))) {
                result_score = max(m[i][len_b][0], max(m[i][len_b][1], m[i][len_b][2]));
                result_col = len_b;
                result_row = i;
            }
        }
    }
    // find biggest element in last row if ending gaps for b are free
    if (conf->bottom==true) {
        for (unsigned int j = 0; j<=len_b; ++j) {
            if (result_score<max(m[len_a][j][0], max(m[len_a][j][1], m[len_a][j][2]))) {
                result_score = max(m[len_a][j][0], max(m[len_a][j][1], m[len_a][j][2]));
                result_col = j;
                result_row = len_a;
            }
        }
    }

    if (m[result_row][result_col][0]>=max(m[result_row][result_col][1], m[result_row][result_col][2])) {
        result_spec = 0;
    }
    else if (m[result_row][result_col][1]>=m[result_row][result_col][2]) {
        result_spec = 1;
    }
    else {
        result_spec = 2;
    }

    if (VERBOSE==true) traceback(a, len_a, b, len_b, trace, result_row, result_col, result_spec);
    return result_score;
}



