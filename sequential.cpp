#include <iostream>
#include <ctime>
#include <cstdlib>
#include "timer.h"

using namespace std;

const int N = 1000;         // matrix size
const int MAX = 1000;       // max double limit
double A[N][N];
double B[N][N];
double AB[N][N];
double start_time;
double end_time;

/* initialize A and B matrices with random doubles */
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



void sequenceMul()
{
    int i,j,k;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            AB[i][j]=0;
            for(k=0; k<N; k++)
            {
                AB[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
}

int main()
{
    cout << "Sequential Algorithm" << endl;
    cout << "Matrix Size : " << N << endl;
    init();

    GET_TIME(start_time)
    sequenceMul();
    GET_TIME(end_time)

    cout << "Running Time : " << (end_time-start_time)*1000 << " ms" << endl;

    return 0;
}
