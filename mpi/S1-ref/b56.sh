#!/bin/sh
#PJM -N "test"
#PJM -L rscgrp=lecture
#PJM -L node=8
#PJM --mpi proc=448
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o test.lst

export I_MPI_PIN_PROCESSOR_LIST=0-55
mpiexec.hydra -n ${PJM_MPI_PROC} numactl -l ./testa
mpiexec.hydra -n ${PJM_MPI_PROC} ./testa

