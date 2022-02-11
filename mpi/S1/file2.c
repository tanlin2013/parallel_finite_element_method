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
	int num;
	double sum0, sum;
	char filename[80];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	sprintf(filename, "a2.%d", MyRank);
	fp = fopen(filename, "r");
	assert(fp != NULL);

	fscanf(fp, "%d", &num);
	vec = malloc(num * sizeof(double));
	for(i=0;i<num;i++){
		fscanf(fp, "%lf", &vec[i]);
	}

	for(i=0;i<num;i++){
		printf(" %5d%5d%5d%10.0f\n", MyRank, i+1, num, vec[i]);
	}
	printf("\n");

	Count = calloc(PeTot, sizeof(int));
	CountIndex = calloc(PeTot+1, sizeof(int));
	MPI_Allgather(&num, 1, MPI_INT, Count, 1, MPI_INT, MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}
