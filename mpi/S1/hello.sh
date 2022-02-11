#!/bin/sh
#PJM -N "hello"
#PJM -L rscgrp=lecture
#PJM -L node=1
#PJM --mpi proc=4
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o hello.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./a.out
