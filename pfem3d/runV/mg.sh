#!/bin/sh
#PJM -N "pmg"
#PJM -L rscgrp=tutorial
#PJM -L node=8
#PJM --mpi proc=256
#PJM -L elapse=00:10:00
#PJM -g gt00
#PJM -j
#PJM -e err
#PJM -o pmg.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./pmesh

rm wk.*
