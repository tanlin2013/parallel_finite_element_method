#!/bin/sh
#PJM -L rscgrp=lecture
#PJM -L node=1
#PJM -L elapse=00:15:00
#PJM -g gt00
#PJM -j
#PJM -e err
#PJM -o mg.lst

mpiexec.hydra -n 1 ./mgcube < inp_mg
