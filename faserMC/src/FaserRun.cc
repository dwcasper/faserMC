#include "FaserRun.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "FaserDigitizer.hh"
#include "FaserCaloDigitizer.hh"
#include "FaserEvent.hh"
#include "FaserEventInformation.hh"
#include "RootEventIO.hh"


FaserRun::FaserRun() 
{ }

FaserRun::~FaserRun()
{ }

void FaserRun::RecordEvent(const G4Event* g4Evt)
{
  G4cout << "FaserRun::RecordEvent called" << G4endl;

  FaserEventInformation* eventInfo = dynamic_cast<FaserEventInformation*>(g4Evt->GetUserInformation());
  if (eventInfo != nullptr)
  { 
    FaserEvent* faserEvent = eventInfo->GetFaserEvent();
    if (faserEvent != nullptr)
    {
      RootEventIO* rootEventIO = RootEventIO::GetInstance();                                                                          
      rootEventIO->Write(faserEvent);                 
    }
  }
  G4Run::RecordEvent(g4Evt);
}

void FaserRun::Merge(const G4Run* aRun)
{
  G4cout << "FaserRun::Merge called" << G4endl;
  G4Run::Merge(aRun);
}
