#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>

void usage(char* prog_name);
void count_sort();

int main(int argc, char* argv[]) {
    long thread_count;
    long long array_size;
    double start, finish;

    if (argc != 4) usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count < 0) usage(argv[0]);

    GET_TIME(start);
# pragma omp parallel num_threads(thread_count)
    count_sort();

    GET_TIME(finish);
}

/*
 * Function: count_sort
 * Purpose: thread for pragma to run and count sort array
 * In args: 
 * Global out:
 */
void count_sort() {
}

/*
 * Function:    usage
 * Purpose:     print usage if program run incorrectly
 * In args:     prog_name
 */
void usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads> <array size> <array file name>", prog_name);
    fprintf(stderr, "number of threads must be greater than 0\n");
    exit(1);
}
