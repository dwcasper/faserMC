# faserMC


## Prerequisites

The following are required:
 * [Geant4 (version 10.0 or later)](https://geant4.web.cern.ch)
 * [cmake (version 2.6 or later)](https://cmake.org)
 * [ROOT (version 6)](https://root.cern.ch)


## Installation

 1. `git clone https://github.com/asoffa/faserMC`.
 2. `cd faserMC`
 3. Set the needed environment variables in `set_up_environment.sh`.
 4. Set `FASERMC_DO_FULL_BUILD` in `make.sh` to true for the full installation or false for the shared libraries only.
 5. Run `source make.sh` to install the package.


## Running

If the installation steps in the section above are completed successfully, you should be brought to the `faserMC_run` directory.
There, you have the option of running interactively or in batch mode.

To run interactively, run without command-line arguments: `./bin/faser`.

To run in batch mode, use `./bin/faser init_novis.mac outputFile.root`, where
`init_novis.mac` is a sample batch macro file and `outputFile.root` is the name
of the ROOT file where you would like to write the ROOT tree. The only thing you
may want to change in `init_novies.mac` is the number of events (`/run/beamOn N_EVENTS`).

The mass of the Aprime is set in the macro file `faserPhysicsList.mac`.
(For this the name name cannot be changed, as it's hard-coded to be read at job-startup time.)

