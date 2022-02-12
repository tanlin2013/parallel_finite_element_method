#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char **argv){
	int i, neib;
	int MyRank, PeTot;
	double VEC[36];
	int Start_Send, Length_Send;
	int Start_Recv, Length_Recv;

	MPI_Status *StatSR;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);

        Start_Send= 1;

	if(MyRank == 0) {
	  neib= 1;      
          Length_Send= 11;
          Length_Recv= 25;  
          for (i=0;i<36;i++){VEC[i]=100.0+i+1;}}

          if(MyRank == 1) {
	  neib= 0;      
          Length_Send= 25;
          Length_Recv= 11;  
          for (i=0;i<36;i++){VEC[i]=200.0+i+1;}}

        Start_Recv= 1 + Length_Send;

	StatSR = malloc(sizeof(MPI_Status));

        for (i=0;i<36;i++) {
	  printf("%s%2d%5d%8.0f\n", "### before", MyRank, i+1, VEC[i]);}

	MPI_Sendrecv(&VEC[Start_Send-1], Length_Send,  MPI_DOUBLE, neib, 0,
		     &VEC[Start_Recv-1], Length_Recv,  MPI_DOUBLE, neib, 0,
                      MPI_COMM_WORLD, StatSR);

        for (i=0;i<36;i++) {
	  printf("%s%2d%5d%8.0f\n", "### after ", MyRank, i+1, VEC[i]);}
	
	MPI_Finalize();
	return 0;
}

