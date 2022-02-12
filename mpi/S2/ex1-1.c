#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char **argv){
	int neib;
	int MyRank, PeTot;
        double VAL, VALtemp;

	MPI_Status *StatSend, *StatRecv;
	MPI_Request *RequestSend, *RequestRecv;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	StatSend = malloc(sizeof(MPI_Status) * 1);
	StatRecv = malloc(sizeof(MPI_Status) * 1);
	RequestSend = malloc(sizeof(MPI_Request) * 1);
	RequestRecv = malloc(sizeof(MPI_Request) * 1);

	if(MyRank == 0) {neib= 1; VAL= 10.0;}
        if(MyRank == 1) {neib= 0; VAL= 11.0;}

	printf("%s%8d%8.0f\n", "### before", MyRank, VAL);

	MPI_Isend(&VAL, 1, MPI_DOUBLE, neib, 0, 
                   MPI_COMM_WORLD, &RequestSend[0]);
	MPI_Irecv(&VALtemp, 1, MPI_DOUBLE, neib, 0, 
                   MPI_COMM_WORLD, &RequestRecv[0]);
        MPI_Waitall(1, RequestRecv, StatRecv);
        MPI_Waitall(1, RequestSend, StatSend);

        VAL=VALtemp; 

	printf("%s%8d%8.0f\n", "### after", MyRank, VAL);
	
	MPI_Finalize();
	return 0;
}

