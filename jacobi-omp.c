#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main (int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr,"Function needs vector length and number of iterations as input arguments!\n");
		abort();
	}
	
	long int n = atoi(argv[1]), k = atoi(argv[2]);
	
	if (n <=0)
	{
		fprintf(stderr,"Vector length must be positive!\n");
		abort();
	}
	else if (k <= 0)
	{
		fprintf(stderr, "Number of iteration must be positive!\n");
		abort();
	}
	
	double *u, *f, *uold;

	u = (double *) calloc(n,sizeof(double)); // use u to store new result
	uold = (double *) calloc(n,sizeof(double)); // use uold to store previous result
	f = (double *) malloc(sizeof(double)*n);
	
	int i;
	for ( i = 0; i < n; i++)
		f[i] = 1;
	
	double time0 = omp_get_wtime();


	for (i = 0; i < k; i++)
	{
		// jacobi iteration
		{
		if (n == 1)
		{
			u[0] = f[0]/(2*(n+1)*(n+1));
			continue;
		}
	
		u[0] = f[0]/(2*(n+1)*(n+1))+0.5*uold[1];
		
		long int j;
	
		#pragma omp parallel for
		for (j = 1; j < n-1; j++)
			u[j] = f[j]/(2*(n+1)*(n+1))+0.5*uold[j-1]+0.5*uold[j+1];
	
		u[n-1] = f[n-1]/(2*(n+1)*(n+1))+0.5*uold[n-2];
		}
		
		// swap the address of uold and u
		{
		double *temp = uold;
		uold = u;
		u = temp;
		}
	}
	
	double time1 = omp_get_wtime();
	
	printf("Time elapsed: %f seconds.\n", time1-time0);
	
	free(u);
	free(uold);
	free(f);
		
	return 0;
}
