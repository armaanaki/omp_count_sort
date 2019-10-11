#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>
#include <cassert>
#include "read_file.cpp"

void usage(char* prog_name);
void count_sort(double* array, long long array_size);
int compare (const void * a, const void * b);

int main(int argc, char* argv[]) {
    long thread_count;
    long long array_size;
    double start, finish;
    double* array;
    double* array_copy;

    if (argc != 4) usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count < 0) usage(argv[0]);
    array_size = strtol(argv[2], NULL, 10);
	array = read_from_file(argv[3], array_size);
    array_copy = read_from_file(argv[3], array_size);
    // create an exact non-sorted copy to use on qsort

    GET_TIME(start);
# pragma omp parallel num_threads(thread_count)
    count_sort(array, array_size);

    GET_TIME(finish);
    printf("time for count_sort: %e\n", finish - start);

	GET_TIME(start);
	qsort(array_copy, array_size, sizeof(double), compare);
    GET_TIME(finish);
    printf("time for qsort: %e\n", finish - start);
}

/*
 * Function: count_sort
 * Purpose: thread for pragma to run and count sort array
 * In args: 
 * Global out:
 */
void count_sort(double* array, long long array_size) {
    int thread_count = omp_get_num_threads();
    int thread_rank = omp_get_thread_num();
    long long n_per_thread = array_size / thread_count;
    long long my_start = thread_rank * n_per_thread;
    long long my_end = my_start + n_per_thread;

    for (int i = my_start; i < my_end; i++) {
        long long count = 0;
        for (int j = 0; j < array_size; j++) {
            if (array[i] < array[j])
                count++;
            else if (array[j] == array[i] && j < i)
                count++;

# pragma omp critical 
{
            double temp = array[count];
            array[count] = array[i];
            array[i] = temp;
}

        }
    }
}

/*
 * Function:    usage
 * Purpose:     print usage if program run incorrectly
 * In args:     prog_name
 */
void usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads> <array size> <array file name>\n", prog_name);
    fprintf(stderr, "number of threads must be greater than 0\n");
    exit(1);
}

int compare (const void * a, const void * b) {
      return ( *(int*)a - *(int*)b );
}
