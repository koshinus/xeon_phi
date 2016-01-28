#include<stdio.h>
#include<omp.h>

double my_abs(double num)
{
	return (num > 0)? num : -num;
}

int main()
{
	double real_pi = 3.14159265358979;
	double my_pi = 0;
	double step_sign = 1;
	unsigned long long i = 1;
	double start = omp_get_wtime();
	while(my_abs(real_pi - my_pi) > 0.00000001)
	{
		my_pi += 4.0/i * step_sign;
		step_sign *= -1;
		i += 2;
	}
	double end = omp_get_wtime();
	printf("%lf\n", end-start);
	return 0;
}
