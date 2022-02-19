#!/bin/sh
#PJM -N "Flatx28x2"
#PJM -L rscgrp=lecture7
#PJM -L node=1
#PJM --mpi proc=56
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o k01x28x2_0001.lst

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

