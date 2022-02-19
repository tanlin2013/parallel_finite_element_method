#!/bin/sh
#PJM -N "HB24x01x2"
#PJM -L rscgrp=debug
#PJM -L node=2
#PJM --mpi proc=64
#PJM -L elapse=00:15:00
#PJM -g pz0088
#PJM -j
#PJM -e err
#PJM -o test4.lst

mpiexec.hydra -n ${PJM_MPI_PROC} ./sol
