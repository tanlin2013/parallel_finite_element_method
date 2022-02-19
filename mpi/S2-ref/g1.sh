#!/bin/sh
#PJM -N "test"
#PJM -L rscgrp=tutorial
#PJM -L node=1
#PJM --mpi proc=32
#PJM -L elapse=00:15:00
#PJM -g gt00
#PJM -j
#PJM -e err
#PJM -o y032.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./1df
mpiexec.hydra -n ${PJM_MPI_PROC} ./1d2f
mpiexec.hydra -n ${PJM_MPI_PROC} ./1dc
mpiexec.hydra -n ${PJM_MPI_PROC} ./1d2c

