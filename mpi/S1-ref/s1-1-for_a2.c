#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int main(int argc, char **argv){
	int i;
	int PeTot, MyRank;
	MPI_Comm SolverComm;
	double *vec, *vec2;
	int * Count, CountIndex;
	int n;
	double sum0, sum;
	char filename[80];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
	MPI_Comm_dup (MPI_COMM_WORLD, &SolverComm);

	sprintf(filename, "a2.%d", MyRank);
	fp = fopen(filename, "r");
	assert(fp != NULL);

	fscanf(fp, "%d", &n);
	vec = malloc(n * sizeof(double));
	for(i=0;i<n;i++){
		fscanf(fp, "%lf", &vec[i]);
	}
	sum0 = 0.0;
	for(i=0;i<n;i++){
		sum0 += vec[i] * vec[i];
	}

	MPI_Allreduce(&sum0, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	sum = sqrt(sum);

	if(!MyRank) printf("%27.20E\n", sum);

	MPI_Finalize();

	return 0;
}
