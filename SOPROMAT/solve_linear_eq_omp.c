/*for (my $i = 0; $i < $obj1->{row_num}; $i++)
{
	for (my $j = 0; $j < $obj2->{column_num}; $j++)
	{
		for (my $k = 0; $k < $obj1->{column_num}; $k++)
		{
			$obj3_data[$i][$j] += ($obj1_data[$i][$k]*$obj2_data[$k][$j]);
		}
	}
}*/

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
                				    
void mat_x_mat_omp()
{
	char *results_file = "mult_mat_vec_omp.txt";
	FILE *res;
	if((res=fopen(results_file, "w"))==NULL)
	{
		printf("Can't open file %s.\n", results_file);
		exit(1);
	}
	#pragma omp parallel for
	for(int n = 10; n < ROW; n*=10)
	{
		#pragma omp parallel for
		for(int m = 10; m < COL; m*=10)
		{
			double **A = get_matrix(m,n);
			double **B = get_matrix(n,m);
			double **C = (double **)malloc(m*sizeof(double*));
			for(int s = 0; s < m; s++)
				C[s] = (double *)calloc(m,sizeof(double));
			double start = omp_get_wtime();
			#pragma omp parallel for
			for (int i = 0; i < m; i++)
				#pragma omp parallel for
				for (int j = 0; j < m; j++)
					#pragma omp parallel for
					for (int k = 0; k < n; k++)
						C[i][j] += A[i][k]*B[k][j];
			double end = omp_get_wtime();
			fprintf(res, "%lf\n", end-start);
			for(int s = 0; s < m; s++)
				free(A[s]); free(C[s]);
			for(int s = 0; s < n; s++)
				free(B[s]);
			free(A); free(B); free(C);
		}
	}
	fclose(res);
	return 0;
}
