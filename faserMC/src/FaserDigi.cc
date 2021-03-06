#include "FaserDigi.hh"
#include "FaserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

const FaserDetectorConstruction* FaserDigi::fDetectorConstruction = nullptr;

G4ThreadLocal G4Allocator<FaserDigi> *faserDigiAllocator = 0;

FaserDigi::FaserDigi()
  : G4VDigi(),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fTransform(G4AffineTransform()),
    fCharge(0.0)
{ }

FaserDigi::~FaserDigi()
{ }

FaserDigi::FaserDigi(const FaserDigi& right)
  : G4VDigi()
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fTransform = right.fTransform;
  fCharge = right.fCharge;
}

const FaserDigi& FaserDigi::operator=(const FaserDigi& right)
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fTransform = right.fTransform;
  fCharge = right.fCharge;

  return *this;
}

int FaserDigi::operator==(const FaserDigi& right) const
{
  return ( fPlaneID == right.fPlaneID 
	&& fModuleID == right.fModuleID 
	&& fSensorID == right.fSensorID
        && fRowID == right.fRowID
	&& fStripID == right.fStripID
	&& fCharge == right.fCharge ) ? 1 : 0;
}

G4Colour FaserDigi::QtoC()
{
  if (fCharge <=0) return G4Colour(0,0,0);
  G4double q = std::min(log(std::max(1.0,fCharge))/log(30000.0),1.0);
  G4double h = 300.0 * (1 - q); // from 300 at q=0 to 0 at q=1 (purple->blue->green->...->red)
  G4double x = 1.0 - fabs(std::fmod(h/60.0, 2.0)-1.0);
  if (h < 60) return G4Colour(1.0,x,0);
  if (h < 120) return G4Colour(x,1.0,0);
  if (h < 180) return G4Colour(0,1.0,x);
  if (h < 240) return G4Colour(0,x,1.0);
  if (h < 300) return G4Colour(x,0,1.0);
  return G4Colour(1.0,0,x);
}

void FaserDigi::Draw()
{ 
  if (fCharge <= 0) return;
  if (fDetectorConstruction == nullptr)
  {
    G4MTRunManager* master = G4MTRunManager::GetMasterRunManager();
    if (master != nullptr)
    {
      fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(master->GetUserDetectorConstruction());
    }
    else
    {
      fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(
					  G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    }
  }
  if (fDetectorConstruction == nullptr)
  {
    G4cout << "FaserDigi::Draw unable to obtain detetor construction" << G4endl;
  }
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager != nullptr)
  {
    // wasted a couple days figuring out that the rotation (but not translation!) needs to be inverted for correct display...
    G4Transform3D trans(fTransform.NetRotation().inverse(), fTransform.NetTranslation());
    G4VisAttributes attribs;
    const G4LogicalVolume* strip = fDetectorConstruction->GetTrackerStrip();
    const G4VisAttributes* pVA = strip->GetVisAttributes();
    if (pVA != nullptr) attribs = *pVA;
    attribs.SetColour(QtoC());
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*strip, attribs, trans);
  }

}

void FaserDigi::Print()
{ }
