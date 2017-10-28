# Your One Get Remaining Time library
This library provides functions to query a resource manager
for the time remaining in a job.

It supports:
- SLURM
- MOAB
- LCRM
- AIX w/ SLURM

See [src/yogrt.h](https://github.com/LLNL/libyogrt/blob/master/src/yogrt.h) for documentation.

## Build
To build configure after cloning the repo:

    ./bootstrap

To build and install:

    mkdir build
    cd build
    ../configure
    make
    make install

To build a release tarball, update version in configure.ac file:

    mkdir build
    cd build
    ../configure
    make dist
