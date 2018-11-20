#include "FaserEventInformation.hh"
#include "FaserEvent.hh"

FaserEventInformation::FaserEventInformation(FaserEvent* event) : fEvent(event)
{ }

FaserEventInformation::~FaserEventInformation()
{
    delete fEvent;
}

FaserEvent* FaserEventInformation::GetFaserEvent() const
{
    return fEvent;
}

void FaserEventInformation::Print() const
{
    G4cout << "Strip hits: " << fEvent->TrackerHits().size() << 
    ", Strip digits: " << fEvent->TrackerDigis().size() << 
    ", Strip clusters: " << fEvent->Clusters().size() << 
    ", Space points: " << fEvent->SpacePoints().size() << G4endl;
  
  G4cout << "Sampler hits: " << fEvent->SamplerHits().size() << 
    ", Sampler digits: " << fEvent->SamplerDigis().size() << G4endl;
  G4cout << "Calo hits: " << fEvent->CaloHits().size() << 
    ", Calo digits: " << fEvent->CaloDigis().size() << G4endl;
}