#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "mkl.h"
#define EXPERIMENTS_NUM  10
#define ROW 1000
#define COL 1000

double ** get_matrix(int row, int col)
{
	double **mat;
	mat = (double **)malloc(row*sizeof(double*));
	for(int i = 0; i < row; i++)
	{
		VSLStreamStatePtr stream;
		vslNewStream( &stream, VSL_BRNG_MT19937, i*time(0) );
		mat[i] = (double *)malloc(col*sizeof(double*));
    		vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
		          ARRAY_SIZE, mat[i], 1.0, 3.0 );
		vslDeleteStream( &stream );
	}
	return mat;
}

double * get_vector(int size, int i)
{
	double *vec;
	VSLStreamStatePtr stream;
	vslNewStream( &stream, VSL_BRNG_MT19937, i*time(0) );
	vec = (double *)malloc(size*sizeof(double*));
	vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
	          size, vec, 1.0, 3.0 );
	vslDeleteStream( &stream );
	return vec;
}
                				    
void mult_mat_vec_omp()
{
	char *results_file = "solve_triangle_eq_omp.txt";
	FILE *res;
	if((res=fopen(results_file, "w"))==NULL)
	{
		printf("Can't open file %s.\n", results_file);
		exit(1);
	}
	#pragma omp parallel for
	for(int i = 10; i < ROW; i*=10)
	{
		#pragma omp parallel for
		for(int j = 10; j < COL; j*=10)
		{
			double **A = get_matrix(i,j);
			double 	*X = get_vector(j,i*j);
			double start = omp_get_wtime();
			#pragma omp parallel for
			for (int s=i-1; s>=0; s--) 
			{
				X[s] = A[s][i]/A[s][s];
				#pragma omp parallel for
				for (int k=s-1;k>=0; k--)
				    A[k][i] -= A[k][s] * X[s];
			}
			double end = omp_get_wtime();
			fprintf(res, "%lf\n", end-start);
			free(X);
			for(int s = 0; s < i; s++)
				free(A[s]);
			free(A);
		}
	}
	fclose(res);
	return 0;
}
