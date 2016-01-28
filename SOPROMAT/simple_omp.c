#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "mkl.h"
#define EXPERIMENTS_NUM  10
#define ARRAY_SIZE 1000

void simple_omp()
{
    int i,j,k;
    char *results_file = "simple_omp.txt";
    FILE *res;
    if((res=fopen(results_file, "w"))==NULL)
    {
        printf("Can't open file %s.\n", results_file);
        exit(1);
    }
    #pragma omp parallel for reduction(*:i)
    for(i = 10; i < ARRAY_SIZE; i*=10)
    {
        VSLStreamStatePtr stream;
        vslNewStream( &stream, VSL_BRNG_MT19937, time(0) );
        double *ar1;double *ar2;double *ar3;double *ar4;double *ar5;double *ar6;
        //double *ar1, ar2, ar3, ar4, ar5, ar6;
        ar1 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        ar2 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        ar3 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        ar4 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        ar5 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        ar6 = (double *)malloc(ARRAY_SIZE*sizeof(double));
        vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                        ARRAY_SIZE, ar1, 1.0, 3.0 );
        vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
    	        	ARRAY_SIZE, ar2, 1.0, 3.0 );
        double start = omp_get_wtime();
        #pragma omp parallel for
        for(j = 0; j < EXPERIMENTS_NUM; j++)
        {
    	    #pragma omp parallel for shared(ar1,ar2,ar3,ar4,ar5,ar6)
            for(k = 0; k < ARRAY_SIZE; k++)
    	    {
        	ar3[k] = ar1[k] + ar2[k];
            	ar4[k] = ar1[k] - ar2[k];
            	ar5[k] = ar1[k] * ar2[k];
            	ar6[k] = sqr(ar1[k]);
            }
	}
        double end = omp_get_wtime();
        free(ar1); free(ar2); free(ar3); free(ar4); free(ar5); free(ar6);
        fprintf(res, "%lf", end-start);
        vslDeleteStream( &stream );
    }
    fclose(res);
}
