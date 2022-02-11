#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){
	int i;
	int PeTot, MyRank;
	MPI_Comm SolverComm;
	double TimeStart, TimeEnd;
	long ClockStart, ClockEnd;
	double WallTime;
	volatile double a;
	const long nloop = 1000000000;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	TimeStart = MPI_Wtime();
	for(i=0;i<nloop;i++){
		a = 1.0;
	}
	TimeEnd = MPI_Wtime();
	printf("%5d%16.6E\n", MyRank, TimeEnd - TimeStart);

	MPI_Finalize();

	return 0;
}

