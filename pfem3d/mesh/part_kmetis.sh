#!/bin/sh
#PJM -N "K-MeTiS"
#PJM -L rscgrp=lecture7
#PJM -L node=1
#PJM -L elapse=00:15:00
#PJM -g gt37
#PJM -j
#PJM -e err
#PJM -o test.lst

module load metis/4.0.3
mpiexec.hydra -n 1 ./part < inp_kmetis
rm work.*
