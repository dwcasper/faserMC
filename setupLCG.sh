#!/bin/bash
#
LCG_RELEASE=$1
if [[ -n "$2" ]]; then
    SYS_CONFIG=$2
else
    SYS_CONFIG="x86_64-slc6-gcc7-opt"
fi
#
# CMake
#
CMake_VERSION=`ls /cvmfs/sft.cern.ch/lcg/releases/$LCG_RELEASE/CMake`
source /cvmfs/sft.cern.ch/lcg/releases/$LCG_RELEASE/CMake/$CMake_VERSION/$SYS_CONFIG/CMake-env.sh
#
# Geant4
#
Geant4_VERSION=`ls /cvmfs/sft.cern.ch/lcg/releases/$LCG_RELEASE/Geant4`
source /cvmfs/sft.cern.ch/lcg/releases/$LCG_RELEASE/Geant4/$Geant4_VERSION/$SYS_CONFIG/Geant4-env.sh
source geant4.sh
#
# avoids weird error/warning
#
export FONTCONFIG_PATH=$FONTCONFIG_HOME/etc/fonts

