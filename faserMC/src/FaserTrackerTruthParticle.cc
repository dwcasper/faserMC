#include "FaserTrackerTruthParticle.hh"


//------------------------------------------------------------------------------

FaserTrackerTruthParticle::FaserTrackerTruthParticle()
  : FaserTrackerTruthParticle(0, 0, 0, TVector3{}, TLorentzVector{})
{
}

//------------------------------------------------------------------------------

FaserTrackerTruthParticle::FaserTrackerTruthParticle(int trackID_, int parentID_, int pdgCode_,
    const TVector3 & vertex_, const TLorentzVector & fourMomentum_)
  : trackID { trackID_ }
  , parentID { parentID_ }
  , pdgCode { pdgCode_ }
  , vertex { vertex_ }
  , fourMomentum { fourMomentum_ }
{ }

FaserTrackerTruthParticle::~FaserTrackerTruthParticle()
{
}

