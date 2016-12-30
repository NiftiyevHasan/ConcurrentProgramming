#include <iostream>
#include <ctime>
#include <cstdlib>
#include "timer.h"
#include <omp.h>
#include <math.h>

using namespace std;

const int N = 1000;
const int MAX = 1000;
const int SAMPLE = 30;
const double Z = 1.96;
const double R = 0.05;
double A[N][N];
double B[N][N];
double BT[N][N];
double AB[N][N];
double start_time;
double end_time;
int i,j,k;

/* print AB matrix (this is for validation) */
void print()
{
    int i,j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            cout << AB[i][j] << "\t" ;
        }

        cout << endl;
    }
}

/* Set all elements of AB matrix to 0 */
void clean()
{
    int i,j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            AB[i][j] = 0;
        }
    }
}

/* sequential algorithm */
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

/* parallel algorithm (based on OpenMP parallel for) */
void parallelMul()
{
    int i,j,k;
    #pragma omp parallel for private(i, j, k) shared(A,B,AB)
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            AB[i][j] = 0;
            for(k=0; k<N; k++)
            {
                AB[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
}

/* Optimized Algorithm (based on cache optimization with parallel for) */
inline void optimizedMul()
{
    #pragma omp parallel for private(i, j) shared(B,BT)
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            BT[i][j]=B[j][i];
        }
    }

    double temp=0;
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

/* initialize A and B matrices with random doubles values */
void init()
{
    srand(time(NULL));
    int i = 0;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            A[i][j] = (double)rand()*MAX/RAND_MAX;
            B[i][j] = (double)rand()*MAX/RAND_MAX;
        }
    }
}

/* return mean */
double mean(double arr[], int sz)
{
    double sum = 0;
    int i;
    for(i = 0; i < sz; i++)
    {
        sum = sum + arr[i];
    }
    return sum/sz;
}

/* return statndard deviation */
double sd(double arr[], double mean , int sz)
{
    double dev = 0;
    int i;
    for(i = 0; i < sz; i++)
    {
        dev += pow((arr[i]-mean),2);
    }

    return sqrt(dev/sz);
}

/*
 *  This program shows all running time of sequential, parallel and optimized vertion for given matrix size
 *  Also calculate number of sample sizes for each version.
 *
 */

int main()
{
    init();
    double TS[SAMPLE],TP[SAMPLE],TO[SAMPLE];
    double MS,MP,MO,SS,SP,SO;
    int NS,NP,NO;
    int i;
    for(i=0; i<SAMPLE; i++)
    {
        cout << "Iteration : " << i+1 << endl;

        GET_TIME(start_time)
        sequenceMul();
        GET_TIME(end_time)
        TS[i]=end_time-start_time;

        GET_TIME(start_time)
        parallelMul();
        GET_TIME(end_time)
        TP[i]=end_time-start_time;

        GET_TIME(start_time)
        optimizedMul();
        GET_TIME(end_time)
        TO[i]=end_time-start_time;
    }

    /* calculate means */
    MS = mean(TS,SAMPLE);
    MP = mean(TP,SAMPLE);
    MO = mean(TO,SAMPLE);

    /* calculate standard deviations */
    SS = sd(TS,MS,SAMPLE);
    SP = sd(TP,MP,SAMPLE);
    SO = sd(TO,MO,SAMPLE);

    /* calculate number of samples z = 1.96 and r = 5% */
    NS = (int)pow(((Z*SS)/(R*MS)),2) + 1;
    NP = (int)pow(((Z*SP)/(R*MP)),2) + 1;
    NO = (int)pow(((Z*SO)/(R*MO)),2) + 1;

    cout << endl;

    cout << "Mean Sequential : " << MS*1000 << endl;
    cout << "Mean Parallel : " << MP*1000 << endl;
    cout << "Mean Optimized : " << MO*1000 << endl;

    cout << endl;

    cout << "SD Sequential : " << SS*1000 << endl;
    cout << "SD Parallel : " << SP*1000 << endl;
    cout << "SD Optimized : " << SO*1000 << endl;

    cout << endl;

    cout << "Samples Sequential : " << NS << endl;
    cout << "Samples Parallel : " << NP << endl;
    cout << "Samples Optimized : " << NO << endl;

    return 0;
}
