#!/bin/bash

# Script for automated installation of `faserMC` simulation package. Before
# running, set the environment variables in `set_up_environment.sh` to the
# correct locations. Then, set `FASERMC_DO_FULL_BUILD` below to `true` to install
# the full simulation package or `false` to install the shared libraries only.

# For Geant4 setup, see https://twiki.cern.ch/twiki/bin/view/Main/GEANT4

################################################################################
# Configuration settings:

export FASERMC_DO_FULL_BUILD=true  # Set to `true` or `false`

# End of configurtion settings
################################################################################


printUsage() {
    echo "Usage: source make.sh [--clean]"
}

#-------------------------------------------------------------------------------

makeFaserMC() {

    if [ -z $FASERMC_DIR ]; then
        local faserMCDir="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"
        echo 'Sourcing `'"${faserMCDir}/set_up_environment.sh"'` to set up the environment...'
        source ${faserMCDir}/set_up_environment.sh
    fi

    if [ "$2" != "" ]; then
        printUsage
        return 1
    fi
    if [ "$1" == "--clean" ]; then
        echo "Wiping ${FASERMC_DIR}/faserMC_build, ${FASERMC_DIR}/faserMC_run..."
        rm -rf ${FASERMC_DIR}/faserMC_build ${FASERMC_DIR}/faserMC_run
    elif [ "$1" != "" ]; then
        printUsage
        return 1
    fi

    cd $FASERMC_DIR
    echo "Now working in ${FASERMC_DIR}..."
    
    buildDir="${FASERMC_DIR}/faserMC_build"
    runDir="${FASERMC_DIR}/faserMC_run"
    mkdir -p $buildDir $runDir
    cd $buildDir
    echo "Now working in ${buildDir}..."
    
    cmake -DGeant4_DIR="${GEANT4_INSTALL_DIR}/lib64/Geant4-10.4.9" -DCMAKE_INSTALL_PREFIX="$runDir" $FASERMC_DIR/faserMC
    make && make install
    
    echo "Setup complete."

    cd $runDir
    echo "Now working in ${runDir}..."
    echo "To run the FASER software, run ./bin/faser"
}

makeFaserMC $@

