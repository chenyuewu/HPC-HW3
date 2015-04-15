#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		fprintf(stderr,"Function needs vector length and number of iterations and number of threads as input arguments!\n");
		abort();
	}
	
	long int n = atoi(argv[1]), k = atoi(argv[2]);
	int procs = atoi(argv[3]);	
	if (n <=1)
	{
		fprintf(stderr,"Vector length too small!\n");
		abort();
	}
	else if (n%2)
	{
		fprintf(stderr,"Vector length must be an even number!\n");
		abort();
	}
	else if (k <= 0)
	{
		fprintf(stderr, "Number of iteration must be positive!\n");
		abort();
	}

	int num_t;
	
	double *ur, *ub, *f; // ur for odd nodes, ub for even nodes.

	ur = (double *) calloc(n/2+1,sizeof(double)); // the last one serves as boundary condition, i.e. 0.
	ub = (double *) calloc(n/2+1,sizeof(double)); // the first one serves as boundary condition, i.e. 0.
	f = (double *) malloc(sizeof(double)*n);
	
	int i;
	for ( i = 0; i < n; i++)
		f[i] = 1;
		
	double time0 = omp_get_wtime();
	
	for (i = 0; i < k; i++)
	{
		// gs iteration
		{
		
		long int j;
		#pragma omp parallel
		{
		num_t = omp_get_num_threads();
		// update ur	
		#pragma omp for
		for (j = 0; j < n/2; j++)
			ur[j] = f[j]/(2*(n+1)*(n+1))+0.5*ub[j]+0.5*ub[j+1];
		// update ub
		#pragma omp for
		for (j = 1; j < n/2+1; j++)
			ub[j] = f[n/2+j-1]/(2*(n+1)*(n+1))+0.5*ur[j-1]+0.5*ur[j];
		}
		}
		
	}
		
	double time1 = omp_get_wtime();
	
	printf("Number of Threads: %d.\nTime elapsed: %f seconds.\n", num_t, time1-time0);
	
	free(ur);
	free(ub);
	free(f);
		
	return 0;
	
}
