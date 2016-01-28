#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "mkl.fi"
#include "blas.f90"
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
                				    
void solve_triangle_eq_omp()
{
	char *results_file = "solve_triangle_eq_mkl.txt";
	FILE *res;
	if((res=fopen(results_file, "w"))==NULL)
	{
		printf("Can't open file %s.\n", results_file);
		exit(1);
	}
	#pragma omp parallel for
	for(int i = 10; i < ROW; i*=10)
	{
		double **A = get_matrix(i,i);
		double 	*X = get_vector(i,i);
		double start = omp_get_wtime();
		dtrsv('U', 'N', 'N', i, A, i, X, 1);
		//call dtrsv(uplo, trans, diag, n, a, lda, x, incx)
		double end = omp_get_wtime();
		fprintf(res, "%lf\n", end-start);
		free(X);
		for(int s = 0; s < i; s++)
			free(A[s]);
		free(A);
	}
	fclose(res);
	return 0;
}
