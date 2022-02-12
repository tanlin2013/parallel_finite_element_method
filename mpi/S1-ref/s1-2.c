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
	int *Count, *CountIndex;
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

	Count = calloc(PeTot, sizeof(int));
	CountIndex = calloc(PeTot+1, sizeof(int));
	printf("before %d %d", MyRank, n);
	for(i=0;i<PeTot;i++){
		printf(" %d", Count[i]);
	}
	printf("\n");

	MPI_Allgather(&n, 1, MPI_INT, Count, 1, MPI_INT, MPI_COMM_WORLD);

	printf("after  %d %d", MyRank, n);
	for(i=0;i<PeTot;i++){
		printf(" %d", Count[i]);
	}
	printf("\n");

        CountIndex[0]=0;
	for(i=0;i<PeTot;i++){
		CountIndex[i+1] = CountIndex[i] + Count[i];
	}

	vecg = calloc(CountIndex[PeTot], sizeof(double));

	MPI_Allgatherv(vec, n, MPI_DOUBLE, vecg, Count, CountIndex, MPI_DOUBLE, MPI_COMM_WORLD);

	for(i=0;i<CountIndex[PeTot];i++){
		printf("%8.2f", vecg[i]);
	}
	printf("\n");

	MPI_Finalize();

	return 0;
}
