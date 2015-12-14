#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "mkl.h"
#define PI 3.14159265359
#define EXPERIMENTS_NUM  1000000
#define EXPERIMENT_STEPS 100

__attribute__( (target(mic)) ) int num_of_digit(unsigned long long number)
{
    int count = 0;
    while(number > 0)
    {
        number /= 10;
        count++;
    }
    return count;
}
            
__attribute__( (target(mic)) ) void trajectory(int i, double *xi, double *yi)
{
    char str[11+num_of_digit(i)];
    sprintf(str, "trajectory_%d", i);
    FILE *fp;
    if((fp=fopen(str, "w"))==NULL)
    {
        printf("Can't open file %s.\n", str);
        exit(1);
    }
    VSLStreamStatePtr stream;
    vslNewStream( &stream, VSL_BRNG_MT19937, i*time(0) );
    double random_distance[EXPERIMENT_STEPS];
    double random_direction[EXPERIMENT_STEPS];
    vdRngUniform( VSL_RNG_METHOD_UNIFORM_STD, stream,
                  EXPERIMENT_STEPS, random_direction, 0.0, 2*PI );
    vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
                  EXPERIMENT_STEPS, random_distance, 1.0, 2.0 );
                  
    double x = 0 , y = 0;
    //#pragma omp parallel for reduction(+:x,y)
    for(int step = 0; step < EXPERIMENT_STEPS; step++)
    {
	fprintf(fp, "(%lf;%lf)\n", x, y);
        x += random_distance[step]*cos(random_direction[step]);
        y += random_distance[step]*sin(random_direction[step]);
    }
    *xi = x; 
    *yi = y;
    vslDeleteStream( &stream );
    fclose(fp);
}

__attribute__( (target(mic)) ) void experiment(int i, double *xi, double *yi)
{
            VSLStreamStatePtr stream;
	    vslNewStream( &stream, VSL_BRNG_MT19937, i*time(0) );
	    double random_distance[EXPERIMENT_STEPS];
	    double random_direction[EXPERIMENT_STEPS];
	    vdRngUniform( VSL_RNG_METHOD_UNIFORM_STD, stream,
    	              EXPERIMENT_STEPS, random_direction, 0.0, 2*PI );
	    vdRngGaussian( VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream,
        	      EXPERIMENT_STEPS, random_distance, 1.0, 2.0 );
	    double x = *xi;
	    double y = *yi;
	    #pragma omp parallel for reduction(+:x,y)
	    for(int step = 0; step < EXPERIMENT_STEPS; step++)
	    {
    		x += random_distance[step]*cos(random_direction[step]);
    		y += random_distance[step]*sin(random_direction[step]);
	    }
	    vslDeleteStream( &stream );
	    *xi = x;
	    *yi = y;
}
                				    
int main()
{
    char *results_file = "results.txt";
    FILE *res;
    if((res=fopen(results_file, "w"))==NULL)
    {
	printf("Can't open file %s.\n", results_file);
	exit(1);
    }
    #pragma omp parallel for
    for(int i = 0; i < EXPERIMENTS_NUM; i++)
    {
	int check_var = i%(EXPERIMENTS_NUM/4);
        double xi = 0;
        double yi = 0;
        if(check_var == 0)
	    trajectory(i, &xi, &yi);
	else
        {
	    experiment(i, &xi, &yi);
        }
    	fprintf(res, "%lf %lf\n", xi, yi);
     }
    fclose(res);
    return 0;
}
