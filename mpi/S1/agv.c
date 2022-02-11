#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int main(int argc, char **argv){
	int i;
	int PeTot, MyRank;
	MPI_Comm SolverComm;
	double *vec, *vec2, *vecg;
	int *Rcounts, *Displs;
	int n;
	double sum0, sum;
	char filename[80];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	sprintf(filename, "a2.%d", MyRank);
	fp = fopen(filename, "r");
	assert(fp != NULL);

	fscanf(fp, "%d", &n);
	vec = malloc(n * sizeof(double));
	for(i=0;i<n;i++){
		fscanf(fp, "%lf", &vec[i]);
	}

	Rcounts = calloc(PeTot, sizeof(int));
	Displs = calloc(PeTot+1, sizeof(int));
	printf("before %d %d", MyRank, n);
	for(i=0;i<PeTot;i++){
		printf(" %d", Rcounts[i]);
	}
	printf("\n");

	MPI_Allgather(&n, 1, MPI_INT, Rcounts, 1, MPI_INT, MPI_COMM_WORLD);

	printf("after  %d %d", MyRank, n);
	for(i=0;i<PeTot;i++){
		printf(" %d", Rcounts[i]);
	}
	printf("\n");

        Displs[0] = 0;
	for(i=0;i<PeTot;i++){
		Displs[i+1] = Displs[i] + Rcounts[i];
	}

	printf("Displs  %d ", MyRank);
	for(i=0;i<PeTot+1;i++){
		printf(" %d", Displs[i]);
	}

	printf("\n");

	MPI_Finalize();

	return 0;
}
