This is an extensive 5-days winter school given by Prof. Kengo Nakajima, during Feb 2022.

Docker
------
Bould a Docker container with mpi installed.
To use the scripts in this repo, 
please mount the repo folder in `docker run` as follows. 
```
docker build --no-cache --rm -t openmpi .
docker run --rm -it -v $PWD:/home openmpi
```

References
----------

1. http://nkl.cc.u-tokyo.ac.jp/NTU2022online
2. https://sites.google.com/phys.ncts.ntu.edu.tw/mathphys/2022-hpc-nk
