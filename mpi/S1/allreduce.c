#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv){
	int i,N;
	int PeTot, MyRank;
	double VECp[5], VECs[5]; 
	double sumA, sumR, sum0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

        sumA= 0.0; 
        sumR= 0.0; 

        N=5;
	for(i=0;i<N;i++){
	  VECp[i] = 2.0;
	  VECs[i] = 3.0;
	}

	sum0 = 0.0;
	for(i=0;i<N;i++){
		sum0 += VECp[i] * VECs[i];
	}

	MPI_Reduce(&sum0, &sumR, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Allreduce(&sum0, &sumA, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        printf("before BCAST %5d %15.0F %15.0F\n", MyRank, sumA, sumR);

	MPI_Bcast(&sumR, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        printf("after  BCAST %5d %15.0F %15.0F\n", MyRank, sumA, sumR);

	MPI_Finalize();

	return 0;
}
