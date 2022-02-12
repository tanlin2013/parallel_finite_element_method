#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char **argv){
	int neib;
	int MyRank, PeTot;
        double VAL, VALtemp;
	MPI_Status *StatSR;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	if(MyRank == 0) {neib= 1; VAL= 10.0;}
        if(MyRank == 1) {neib= 0; VAL= 11.0;}

	StatSR = malloc(sizeof(MPI_Status));

	printf("%s%8d%8.0f\n", "### before", MyRank, VAL);

	MPI_Sendrecv(&VAL    , 1, MPI_DOUBLE, neib, 0,
		     &VALtemp, 1, MPI_DOUBLE, neib, 0, MPI_COMM_WORLD, StatSR);

        VAL=VALtemp; 

	printf("%s%8d%8.0f\n", "### after", MyRank, VAL);
	
	MPI_Finalize();
	return 0;
}

