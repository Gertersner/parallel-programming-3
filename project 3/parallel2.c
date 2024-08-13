#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define N 2048
#define B 32 //This is the block size
#define FactorIntToDouble 1.1

double firstMatrix[N][N] = {0.0};
double secondMatrix[N][N] = {0.0};
double matrixMultiResult[N][N] = {0.0};

void matrixMulti()
{
    #pragma omp parallel for collapse(2) //we parallelize the outside loop
    for (int blockRowIndex = 0; blockRowIndex < N; blockRowIndex += B) //run for the duration of the matrix size
    {
        for (int blockColumnIndex = 0; blockColumnIndex < N; blockColumnIndex += B) //also assigning the block;s their respective sections
        {
            for (int innerBlockIndex = 0; innerBlockIndex < N; innerBlockIndex += B)
            {
                for (int i = blockRowIndex; i < blockRowIndex + B; i++)
                {
                    for (int j = blockColumnIndex; j < blockColumnIndex + B; j++)
                    {
                        double resultValue = matrixMultiResult[i][j];
                        for (int k = innerBlockIndex; k < innerBlockIndex + B; k++)
                        {
                            resultValue += firstMatrix[i][k] * secondMatrix[k][j]; //do the multiplication on the specific block
                        }
                        matrixMultiResult[i][j] = resultValue; //store results here in result matrix
                    }
                }
            }
        }
    }
}

void matrixInit()
{
    //I changed the method of keeping track of time
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
    struct timespec start, end;
    matrixInit();
    clock_gettime(CLOCK_MONOTONIC, &start);
 
    matrixMulti();
    clock_gettime(CLOCK_MONOTONIC, &end);
  u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    return 0;
}
