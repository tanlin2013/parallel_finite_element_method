#!/bin/sh
#PJM -N "Validation"
#PJM -L rscgrp=lecture
#PJM -L node=1
#PJM --mpi proc=32
#PJM -L elapse=00:15:00
#PJM -g gt00
#PJM -j
#PJM -e err
#PJM -o t032.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
