#!/bin/sh
#PJM -N "test"
#PJM -L rscgrp=lecture
#PJM -L node=1
#PJM --mpi proc=24
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o test.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./testa
mpiexec.hydra -n ${PJM_MPI_PROC} ./testb

export I_MPI_PIN_PROCESSOR_LIST=0-23
mpiexec.hydra -n ${PJM_MPI_PROC} ./testa
mpiexec.hydra -n ${PJM_MPI_PROC} ./testb
