#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv){
	int i;
	double TimeStart, TimeEnd;
	double sum0, sum;
        double x0, x1, f0, f1;
	int PeTot, MyRank, n;
	int *index;
	double dx;
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	index = calloc(PeTot+1, sizeof(int));

	fp = fopen("input.dat", "r");
	fscanf(fp, "%d", &n);
	fclose(fp);

	if(MyRank==0) printf("%s%8d\n", "N=", n);

	dx = 1.0/n;

	for(i=0;i<=PeTot;i++){
		index[i] = ((long long)i * n)/PeTot;
	}

	TimeStart = MPI_Wtime();
	sum0 = 0.0;
	for(i=index[MyRank]; i<index[MyRank+1]; i++)

	{
		x0 = (double)i * dx;
		x1 = (double)(i+1) * dx;
		f0  = 4.0/(1.0+x0*x0);
		f1  = 4.0/(1.0+x1*x1);
		sum0 += 0.5 * (f0 + f1) * dx;
	}

	MPI_Reduce(&sum0, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	TimeEnd = MPI_Wtime();

	if(!MyRank) printf("%24.16f%24.16f%24.16f\n", sum, 4.0*atan(1.0), TimeEnd - TimeStart);

	MPI_Finalize();
	return 0;
}
