#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char **argv){
	int i, neib;
	int MyRank, PeTot;
	double VEC[36];
	int Start_Send, Length_Send;
	int Start_Recv, Length_Recv;

	MPI_Status *StatSend, *StatRecv;
	MPI_Request *RequestSend, *RequestRecv;

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

	StatSend = malloc(sizeof(MPI_Status) * 1);
	StatRecv = malloc(sizeof(MPI_Status) * 1);
	RequestSend = malloc(sizeof(MPI_Request) * 1);
	RequestRecv = malloc(sizeof(MPI_Request) * 1);

        for (i=0;i<36;i++) {
	  printf("%s%2d%5d%8.0f\n", "### before", MyRank, i+1, VEC[i]);}

	MPI_Isend(&VEC[Start_Send-1], Length_Send, MPI_DOUBLE, neib, 0, 
                   MPI_COMM_WORLD, &RequestSend[0]);
	MPI_Irecv(&VEC[Start_Recv-1], Length_Recv, MPI_DOUBLE, neib, 0, 
                   MPI_COMM_WORLD, &RequestRecv[0]);
        MPI_Waitall(1, RequestRecv, StatRecv);
        MPI_Waitall(1, RequestSend, StatSend);

        for (i=0;i<36;i++) {
	  printf("%s%2d%5d%8.0f\n", "### after ", MyRank, i+1, VEC[i]);}
	
	MPI_Finalize();
	return 0;
}

