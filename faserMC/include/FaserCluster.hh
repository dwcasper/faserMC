#pragma once

#include "FaserDigi.hh"

#include <vector>
#include <memory>
#include "TF1.h"
#include "TH1F.h"

#include "globals.hh"

// Cluster class for grouped Faser digits
//

class FaserCluster {

public:

    FaserCluster();

    FaserCluster(int index, std::vector<FaserDigi*> & digis);

    FaserCluster(const FaserCluster& right);

    FaserCluster& operator=(const FaserCluster& right);

    virtual ~FaserCluster();

    std::vector<FaserDigi*>& Digis() { return fDigis; }

    G4int     Plane()  const { return (fDigis.size() > 0 ? fDigis[0]->Plane() : -1); }
    G4int     Module() const { return (fDigis.size() > 0 ? fDigis[0]->Module() : -1); }
    G4int     Sensor() const { return (fDigis.size() > 0 ? fDigis[0]->Sensor() : -1); }
    G4int     Row()    const { return (fDigis.size() > 0 ? fDigis[0]->Row() : -1);  }

    std::vector<G4int> LocalMaxima() const { return fMaxima; }
    G4double Charge() const { return fCharge; }
    G4double WeightedStrip() const { return fWeightedStrip; }
    G4ThreeVector GlobalPos() const { return fGlobalPos; }

private:
    G4int fIndex;
    std::vector<FaserDigi*> fDigis;

    std::vector<G4int> fMaxima;

    G4double fCharge;
    G4double fWeightedStrip;
    G4ThreeVector fGlobalPos;

    void WeightedAverage();

};

