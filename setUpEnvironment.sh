#!/bin/bash

echo "Setting up environment..."

export FASERMC_DIR="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"

################################################################################
# Location-specific settings: Set the following environment variables to the
# appropriate locations (or comment out if already set).


# cmake executable
export PATH="$HOME/software/cmake-3.11.0-install/bin:$PATH"


# Geant4 setup
echo "Configuring Geant4..."
export GEANT4_DIR="$HOME/software/geant4.10.04.p01"
export GEANT4_BUILD_DIR="$HOME/software/geant4.10.04.p01-build"
export GEANT4_INSTALL_DIR="$HOME/software/geant4.10.04.p01-install"
export LD_LIBRARY_PATH="$GEANT4_INSTALL_DIR/lib64:$LD_LIBRARY_PATH"

echo "Setting up Geant4 datasets..."
source $GEANT4_INSTALL_DIR/share/Geant4-10.4.1/geant4make/geant4make.sh


# faserMC package
echo "Configuring faserMC..."
export LD_LIBRARY_PATH="${FASERMC_DIR}/faserMC_run/lib64:$LD_LIBRARY_PATH"


# ROOT
echo "Configuring ROOT..."
export ROOTSYS="$HOME/software/root"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH:$ROOTSYS"
export PATH="$ROOTSYS/bin:$PATH"


# End of location-specific settings
################################################################################


echo "Done."

