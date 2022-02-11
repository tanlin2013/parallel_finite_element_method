FROM ubuntu:18.04

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

ARG WORKDIR=/home
WORKDIR $WORKDIR

RUN apt-get -y update
RUN apt-get -y install \
    bzip2 \ 
    cmake \
    cpio \
    curl \
    g++ \
    gcc \
    gfortran \
    git \
    gosu \
    libblas-dev \
    liblapack-dev \
    libopenmpi-dev \
    openmpi-bin \
    python3-dev \
    python3-pip \
    virtualenv \
    wget \
    zlib1g-dev \
    vim \
    htop
RUN apt-get -y clean && \
    rm -rf /var/lib/apt/lists/*


CMD [ "/bin/bash" ]
