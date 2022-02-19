#!/bin/sh
#PJM -N "example"
#PJM -L rscgrp=lecture
#PJM -L node=8
#PJM --mpi proc=16
#PJM --omp thread=24
#PJM -L elapse=00:15:00
#PJM -g gt76
#PJM -j
#PJM -e err
#PJM -o testH.lst

export KMP_AFFINITY=granularity=fine,compact
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol2
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol2
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol2
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol2
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol2
