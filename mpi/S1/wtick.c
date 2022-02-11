#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){
	int i;
	int PeTot, MyRank;
	MPI_Comm SolverComm;
	double Time;
	double WallTime;
	volatile double a;
	const long nloop = 1000000000;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	Time = MPI_Wtick();
	printf("%5d%16.6E\n", MyRank, Time);

	MPI_Finalize();

	return 0;
}

