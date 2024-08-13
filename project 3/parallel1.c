#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 2048
#define FactorIntToDouble 1.1

double firstMatrix[N][N] = {0.0};
double secondMatrix[N][N] = {0.0};
double matrixMultiResult[N][N] = {0.0};

void matrixMulti()
{
    #pragma omp parallel for //added here
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            double resultValue = 0;
            #pragma omp simd //next addition 
            for (int transNumber = 0; transNumber < N; transNumber++)
            {
                resultValue += firstMatrix[row][transNumber] * secondMatrix[transNumber][col];
            }
            matrixMultiResult[row][col] = resultValue;
        }
    }
}

void matrixInit()
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            srand(row + col);
            firstMatrix[row][col] = (rand() % 10) * FactorIntToDouble;
            secondMatrix[row][col] = (rand() % 10) * FactorIntToDouble;
        }
    }
}

int main()
{
    //Changed the method of collecting time information 
    struct timespec start, end;
    matrixInit();
    clock_gettime(CLOCK_MONOTONIC, &start);
 
    matrixMulti();
    clock_gettime(CLOCK_MONOTONIC, &end);
  u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    return 0;
}

