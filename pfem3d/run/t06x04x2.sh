#!/bin/sh
#PJM -N "HB06x04x2"
#PJM -L rscgrp=lecture7
#PJM -L node=1
#PJM --mpi proc=8
#PJM --omp thread=6
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o t06x04x2_0001.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1

export KMP_AFFINITY=granularity=fine,compact
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol1

