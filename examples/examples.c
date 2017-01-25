//
// examples.pyinterface
//
// shows various examples of alignments and overlaps directly calling the library or source code files
//
// Created by andreas on 1/14/17.
//

#include "../src/overlap.h"

void example_1()
{
    alignconfig_t conf = create_default_alignconfig();
    char a[] = "aab";
    char b[] = "aaa";
    printf("Score: %i\n\n", align(a, strlen(a), b, strlen(b), conf));
    destroy_alignconfig(conf);
}

void example_2()
{
    alignconfig_t conf = create_default_alignconfig();
    char a[] = "ab";
    char b[] = "abaa";
    printf("Score: %i\n\n", align(a, strlen(a), b, strlen(b), conf));
    destroy_alignconfig(conf);
}

void example_naive_overlap()
{
    char a[] = "aaab";
    char b[] = "abaa";
    printf("Naive method\nOverlap Score: %i\n\n", naive_overlap(a, strlen(a), b, strlen(b)));
}

void example_overlap()
{
    char a[] = "aaab";
    char b[] = "abaa";
    printf("Overlap Score: %i\n\n", overlap(a, strlen(a), b, strlen(b)));
}

int main()
{
    example_1();
    example_2();
    example_naive_overlap();
    example_overlap();
    return 0;
}