#!/bin/sh
#PJM -N "VIS"
#PJM -L rscgrp=tutorial
#PJM -L node=8
#PJM --mpi proc=256
#PJM -L elapse=00:15:00
#PJM -g gt00
#PJM -j
#PJM -e err
#PJM -o testV.lst

export KMP_AFFINITY=granularity=fine,compact
mpiexec.hydra -n ${PJM_MPI_PROC} ./solv

