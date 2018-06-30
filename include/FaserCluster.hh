#pragma once

#include "FaserDigi.hh"

#include <vector>
#include <memory>
#include "TF1.h"
#include "TH1F.h"

// Cluster class for grouped Faser digits
//

class FaserCluster {

    struct ClusterFit {
        double  chiSquared;
        double  stripPosition;
        double  positionError;
        double  width;
        double  amplitude;
        int     nDoF;
    };


    int fIndex;

    int fPlane;
    int fModule;
    int fSensor;
    int fRow;

    double fCharge;
    std::vector<FaserDigi*> fDigis;
    int fNMaxima;
    std::vector<int> fMaxima;
    TH1F fH;
    TString fFitName;
    std::vector<ClusterFit> fFitResults;


public:

    FaserCluster(int index, std::vector<FaserDigi*> & digis);

    virtual ~FaserCluster() {
    }

    std::vector<FaserDigi*>& Digis() { return fDigis; }

    int  Plane()  const { return fPlane;  }
    int  Module() const { return fModule; }
    int  Sensor() const { return fSensor; }
    int  Row()    const { return fRow;    }

    double Charge() const { return fCharge; }
    std::vector<int> LocalMaxima() const { return fMaxima; }
    void Draw() { fH.Draw(); Fit(); }
    std::vector<ClusterFit> Fit();
    std::vector<ClusterFit> WeightedAverage();


private:

    std::shared_ptr<TF1> threeParGauss;
    std::shared_ptr<TF1> fivePar2Gauss;
    std::shared_ptr<TF1> sixPar2Gauss;

};

