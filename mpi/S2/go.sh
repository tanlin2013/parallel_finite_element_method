#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:mpiprocs=16
#PBS -Wgroup_list=gt00
#PBS -l walltime=00:05:00
#PBS -e err
#PBS -o test.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export I_MPI_PIN_DOMAIN=socket
export I_MPI_PERHOST=16
mpirun ./impimap.sh ./a.out
