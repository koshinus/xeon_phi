#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "mkl.h"
#define EXPERIMENTS_NUM  10
#define ARRAY_SIZE 1000000
                				    
void hard_mkl()
{
	char *results_file = "hard_mkl.txt";
	FILE *res;
	if((res=fopen(results_file, "w"))==NULL)
	{
		printf("Can't open file %s.\n", results_file);
		exit(1);
	}
	#pragma omp parallel for
	for(int i = 10; i < ARRAY_SIZE; i*=10)
	{
		VSLStreamStatePtr stream;
		vslNewStream( &stream, VSL_BRNG_MT19937, time(0) );
		double * ar1, ar2, ar3, ar4; 
		ar1 = (double *)malloc(ARRAY_SIZE*sizeof(double));
		ar2 = (double *)malloc(ARRAY_SIZE*sizeof(double));
		ar3 = (double *)malloc(ARRAY_SIZE*sizeof(double));
		ar4 = (double *)malloc(ARRAY_SIZE*sizeof(double));
    		vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                  	ARRAY_SIZE, ar1, 1.0, 3.0 );
    		vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                  	ARRAY_SIZE, ar2, 1.0, 3.0 );
    		vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                  	ARRAY_SIZE, ar3, 1.0, 3.0 );
    		vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                  	ARRAY_SIZE, ar4, 1.0, 3.0 );
		double start = omp_get_wtime();
		#pragma omp parallel for
		for(int j = 0; j < EXPERIMENTS_NUM; j++)
		{
			vdcos (ARRAY_SIZE, ar1, ar1);
			vdln  (ARRAY_SIZE, ar2, ar2);
			vdpow (ARRAY_SIZE, ar3, ar4, a3);
			vdcosh(ARRAY_SIZE, ar4, ar4);
		}
		double end = omp_get_wtime();
		free(ar1); free(ar2); free(ar3); free(ar4);
		fprintf(res, "%lf", end-start);
		vslDeleteStream( &stream );
	}
	fclose(res);
	return 0;
}
