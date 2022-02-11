#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

int main(int argc, char **argv){
	int i, N=8, NG=32;
        double ALPHA= 1000.0;
	int PeTot, MyRank;
	double VEC [8];
	double VECg[32];
	char filename[80];
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

	fp = fopen("a1x.all", "r");

	if(!MyRank) for(i=0;i<NG;i++){
		fscanf(fp, "%lf", &VECg[i]);
	}

	MPI_Scatter(VECg, N, MPI_DOUBLE, VEC, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	for(i=0;i<N;i++){
		printf("before %5d %5d %10.0F\n", MyRank, i+1, VEC[i]);
	}

	for(i=0;i<N;i++){
	  VEC[i]= VEC[i] + ALPHA;
	}

	for(i=0;i<N;i++){
        	printf("after  %5d %5d %10.0F\n", MyRank, i+1, VEC[i]);
	}

	MPI_Gather(VEC, N, MPI_DOUBLE, VECg, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (!MyRank) for(i=0;i<NG;i++){
		printf("final  %5d %5d %10.0F\n", MyRank, i+1, VECg[i]);
	}

	MPI_Finalize();

	return 0;
}
