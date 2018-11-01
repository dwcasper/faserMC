#pragma once

#include "TTree.h"

#include <map>
#include "G4String.hh"

class G4UIcommandTree;

//------------------------------------------------------------------------------

class FaserGeometry {

public:
    FaserGeometry();

    // default constructor to retrieve data directly from memory...
    FaserGeometry( G4String path );

    virtual ~FaserGeometry();

    std::map<G4String, std::map<G4String, G4String>> Properties() { return fProperties; }

private:
    std::map<G4String, std::map<G4String, G4String> > fProperties;

    void NavigateCommandTree(G4String path, G4UIcommandTree* cTree);
};

