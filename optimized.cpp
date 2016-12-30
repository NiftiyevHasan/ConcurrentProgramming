#include <iostream>
#include <ctime>
#include <cstdlib>
#include "timer.h"
#include <omp.h>

using namespace std;

const int N = 1000;         // matrix size
const int MAX = 1000;       // max double limit
double A[N][N];
double B[N][N];
double BT[N][N];            // transpose matrix
double AB[N][N];
double start_time;
double end_time;
int i,j,k;

/* initialize A and B matrices with random doubles values */
void init()
{
    srand(time(NULL));
    int i,j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            A[i][j] = (double)rand()*MAX/RAND_MAX;
            B[i][j] = (double)rand()*MAX/RAND_MAX;
        }
    }
}

/* Optimized Algorithm (based on cache optimization with parallel for) */
inline void optimizedMul()
{
    /* calculate transpose of B */
    #pragma omp parallel for private(i, j) shared(B,BT)
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            BT[i][j]=B[j][i];
        }
    }

    double temp=0;              // use temp variable to store intermediate results

    #pragma omp parallel for private(i, j, k, temp) shared(A,BT,AB)
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            temp = 0;
            for(k=0; k<N; k++)
            {
                temp+=A[i][k]*BT[j][k];
            }
            AB[i][j] = temp;
        }
    }
}

int main()
{
    cout << "Optimized Algorithm" << endl;
    cout << "Matrix Size : " << N << endl;
    init();

    GET_TIME(start_time)
    optimizedMul();
    GET_TIME(end_time)

    cout << "Running Time : " << (end_time-start_time)*1000 << " ms" << endl;

    return 0;
}
