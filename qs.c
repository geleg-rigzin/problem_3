#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int compare(const void * x1, const void * x2) {
  return ( *(int*)x1 - *(int*)x2 );
}

void generate_random_vector(int* A, int n) {
    
    int i;
    srand(time(NULL));
    for(i = 0; i < n; ++i) {
        A[i] = rand();
    }
}

   
int main(int argc, char* argv[]) {

    if (argc < 2) {
        return -1;
    }
    int n = atoi(argv[1]);
    int* A = (int*)malloc(n * sizeof(float));
    double start, stop, time_interval;

    generate_random_vector(A, n);
    
    start = omp_get_wtime();    
    qsort(A, n, sizeof(*A), compare);
    stop = omp_get_wtime();
    
    time_interval = stop - start;
    printf("%f\n", time_interval);
    
    free(A);
    
    return 0;
}
