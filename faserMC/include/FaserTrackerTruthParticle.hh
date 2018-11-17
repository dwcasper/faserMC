#pragma once

#include "TVector3.h"
#include "TLorentzVector.h"

//------------------------------------------------------------------------------

struct FaserTrackerTruthParticle
{
  int             trackID;
  int             parentID;
  int             pdgCode;
  TVector3        vertex;
  TLorentzVector  fourMomentum;


  FaserTrackerTruthParticle();
  FaserTrackerTruthParticle(int trackID, int parentID, int pdgCode, const TVector3 & vertex, 
      const TLorentzVector & fourMomentum);

  virtual ~FaserTrackerTruthParticle();

};


