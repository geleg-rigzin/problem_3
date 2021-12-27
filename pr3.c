#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

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

int bynary_search(int key, int* a, int left, int right) {
    int l = left - 1,
        r = right + 1,
        m;
    while (l < r - 1) {
        m = (l + r) / 2;
        if (a[m] < key)
            l = m;
        else
            r = m;
        }
    return r;
}

void bond(int* T, int left1, int right1, 
          int left2, int right2, int* A,
          int left3) {
    int tmp;
    int n1 = right1 - left1 + 1,
        n2 = right2 - left2 + 1;
    if (n1 < n2) {
        tmp = left1; left1 = left2; left2 = tmp;
        tmp = right1; right1 = right2; right2 = tmp;
        tmp = n1; n1 = n2; n2 = tmp;
    }
    if (!n1) {
        return;
    }
    int mid1 = (left1 + right1) / 2,
        mid2 = bynary_search(T[mid1], T, left2, right2),
        mid3 = left3 + (mid1 - left1) + (mid2 - left2);
    A[mid3] = T[mid1];
    #pragma omp task untied 
    {
    bond(T, left1, mid1 - 1, left2, mid2 - 1, A, left3);//parralel
    }
    bond(T, mid1 + 1, right1, mid2, right2, A, mid3 + 1);
}


int* sort(int* A, int n, int num_threads) {
  int i, offset, min_interval, extra;
  int recvcounts[num_threads], displs[num_threads];
  int* B = (int*)malloc(n * sizeof(float));
  min_interval = n / num_threads;
  extra = n % num_threads;
  offset = 0;

  for (i = 0; i < num_threads; i++) {
    if (i < extra)
      recvcounts[i] = min_interval + 1;
    else
      recvcounts[i] = min_interval;
    displs[i] = offset;
    offset = offset + recvcounts[i];
  }
  omp_set_num_threads(num_threads);
  
#pragma omp parallel private(i)
  {

#pragma omp for  
  for (i = 0; i < num_threads; ++i) {
    qsort(A + displs[i], recvcounts[i], sizeof(int), compare);
  }

  int* tmp;
  int s = num_threads, m = 1;
  int rank = omp_get_thread_num();
  int left1, right1, left2, right2, left3;

  while (s > 1) {

     s = s/2 + s%2;
     if((rank == 1) && (num_threads % 2)) {
        memcpy(B + displs[num_threads - 1], A + displs[num_threads - 1], recvcounts[num_threads - 1] * sizeof(int));
     }
     if ((rank%(2*m) == 0) && (num_threads - rank > m)) {
            
            left1 = displs[rank];
            right1 = displs[rank + m] - 1;
            left2 = displs[rank + m];
            left3 = left1;
            
            if(rank + 2*m >= num_threads) {
                right2 = displs[num_threads-1] + recvcounts[num_threads-1] - 1;
            }
            else {
                right2 = displs[rank + 2*m] - 1;
            }           
            bond(A, left1, right1, left2, right2, B, left3);
             }
#pragma omp barrier
            if(!rank) {
                tmp = A; A = B; B = tmp;
            }
#pragma omp barrier
     m = 2*m;
}

  }
    free(B);
    return A;
}
   
int main(int argc, char* argv[]) {

    if (argc < 3) {
        return -1;
    }
    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int* A = (int*)malloc(n * sizeof(float));
    double start, stop, time_interval;
    
    generate_random_vector(A, n);
    
    start = omp_get_wtime();

    omp_set_num_threads(atoi(argv[1]));

    A = sort(A, n, num_threads);
    stop = omp_get_wtime();
    
    time_interval = stop - start;
    printf("%f\n", time_interval);
    
    free(A);
    
    return 0;
}
