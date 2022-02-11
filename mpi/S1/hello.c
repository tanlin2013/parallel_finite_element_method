#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv){
    int n, myid, numprocs, i;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    printf ("Hello World %d\n", myid); 
    MPI_Finalize();

    return 0;
}
