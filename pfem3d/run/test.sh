#!/bin/sh
#PJM -N "example"
#PJM -L rscgrp=lecture
#PJM -L node=8
#PJM --mpi proc=384
#PJM -L elapse=00:15:00
#PJM -g gt76
#PJM -j
#PJM -e err
#PJM -o test.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
