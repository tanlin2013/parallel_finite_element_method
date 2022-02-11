#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){
	int i;
	int PeTot, MyRank;
	MPI_Comm SolverComm;
	double vec[8];
	char FileName[80];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	sprintf(FileName, "a1.%d", MyRank);

	fp = fopen(FileName, "r");
	if(fp == NULL) MPI_Abort(MPI_COMM_WORLD, -1);
	for(i=0;i<8;i++){
		fscanf(fp, "%lf", &vec[i]);
	}

	for(i=0;i<8;i++){
		printf("%5d%5d%10.0f\n", MyRank, i+1, vec[i]);
	}
	
	MPI_Finalize();

	return 0;
}
