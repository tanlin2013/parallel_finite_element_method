#!/bin/sh
#PJM -N "Flatx24x2"
#PJM -L rscgrp=lecture7
#PJM -L node=1
#PJM --mpi proc=48
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o p01x24x2_0001.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./sol

