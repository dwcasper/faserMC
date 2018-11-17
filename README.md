# faserMC


## Prerequisites

Access to CERN cvmfs is required; this is automatically available with CERNVM installations.
You should choose an LCG release, e.g. LCG_94

## Installation

 Create a directory, and change into it; this directory will be referred to as $FASER_ROOT in the following.

1) cd $FASER_ROOT
2) git clone https://github.com/dwcasper/faserMC
3) mkdir build
4) cd build

You will also need to use binaries appropriate for your system configuration. 
You can leave the sysconfig argument blank, which will use the default value of "x86_64-slc6-gcc7-opt"..

5) source ../faserMC/setupLCG.sh (lcg release) (sysconfig)
 
6) source ../faserMC/buildFaser.sh

The program will be installed in $FASER_ROOT/run

7) cd ../run
8) bin/faser

will launch an interactive session. 
Use /run/beamOn 1 in the viewer window to generate a single dark photon event at a time.

9) bin/faser init_novis.mac myDataFile.root

will launch a batch session and generate 20,000 dark photon events. 

You can adjust the number of events generated and the number of CPUs used by editing the .mac file.
Beware that the installed macro files are overwritten each time you rebuild and install.
If you make changes you want to keep, you should change the file name.

The mass of the Aprime is set in the macro file `faserPhysicsList.mac`.
(For this the name cannot be changed, as it's hard-coded to be read at job-startup time.)

You will need to configure LCG with the script each time you log in.

You may also want to add the line:

export LD_LIBRARY_PATH=$FASER_ROOT/run/lib64:$LD_LIBRARY_PATH

to your login script (this assumes that you have also defined FASER_ROOT to point to your installation area).



