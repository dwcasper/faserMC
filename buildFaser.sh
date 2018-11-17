#!/bin/bash
if [[ -z "$FASER_ROOT" ]]; then
    FASER_USE=".."
else
    FASER_USE=$FASER_ROOT
fi
source geant4.sh
cmake -DCMAKE_INSTALL_PREFIX=$FASER_USE/run $FASER_USE/faserMC
make
make install
export LD_LIBRARY_PATH=$FASER_USE/run/lib64:$LD_LIBRARY_PATH
