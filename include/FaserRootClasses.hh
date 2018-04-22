//
// Declaration of classes that need dictionaries
//

#include "FaserDigi.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

std::vector<FaserDigi*> aGarbage;
std::map<G4String, G4AttDef> bGarbage;
std::vector<G4AttValue> cGarbage;

#undef __G4String
