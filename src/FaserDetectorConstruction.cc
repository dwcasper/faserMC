// adapted from Geant4 example

#include "FaserDetectorConstruction.hh"
#include "FaserSensorPlaneConstruction.hh"
#include "FaserGeometryMessenger.hh"
#include "FaserSensorSD.hh"
#include "FaserFieldSetup.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4AutoDelete.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserDetectorConstruction::FaserDetectorConstruction()
  : G4VUserDetectorConstruction(), fGeometryMessenger(new FaserGeometryMessenger(this)),
    fLogicTracker(nullptr), fLogicTrackerPlane(nullptr), 
    sensor_readoutStrips(default_sensor_readoutStrips),
    sensor_stripPitch(default_sensor_stripPitch),
    sensor_stripLength(default_sensor_stripLength),
    sensor_gap(default_sensor_gap),
    sensor_sizeZ(default_sensor_sizeZ),
    sensor_stereoAngle(default_sensor_stereoAngle),
    support_sizeZ(default_support_sizeZ),
    tracker_sensorPlanes(default_tracker_sensorPlanes),
    sampler_sensorPlanes(default_sampler_sensorPlanes),
    sampler_absorberX0(default_sampler_absorberX0),
    detector_samplerLength(default_detector_samplerLength),
    detector_planePitch(default_detector_planePitch),
    detector_decayVolumeLength(default_detector_decayVolumeLength),
    checkOverlaps(true), nist(nullptr), fRegTracker(nullptr), fRegCalorimeter(nullptr),
    fTrackerFactory { new FaserSensorPlaneConstruction(this, "tracker") }, 
    fSamplerFactory { new FaserSensorPlaneConstruction(this, "sampler") }
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserDetectorConstruction::~FaserDetectorConstruction()
{ 
  if (fGeometryMessenger != nullptr) delete fGeometryMessenger;
  if (fTrackerFactory != nullptr) delete fTrackerFactory;
  if (fSamplerFactory != nullptr) delete fSamplerFactory;
}

const G4LogicalVolume* FaserDetectorConstruction::GetTrackerStrip() const
{
  return fTrackerFactory->GetStrip();
}

void FaserDetectorConstruction::ConstructSDandField()
{
  G4String sensorSDName = "Faser/SensorSD";
  FaserSensorSD* aSensorSD = new FaserSensorSD(sensorSDName, 
						  "FaserSensorHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aSensorSD);
  SetSensitiveDetector( "trackerStrip", aSensorSD, true );

/*   G4String samplerSDName = "Faser/SamplerSD";
  FaserSensorSD* aSamplerSD = new FaserSensorSD(samplerSDName,
              "FaserSamplerHitsCollection",
              "Calorimeter");
  G4SDManager::GetSDMpointer()->AddNewDetector(aSamplerSD);
  SetSensitiveDetector( "Strip", aSamplerSD, true);
 */
  // not clear why we need this concurrency stuff...
  if (!fFieldSetup.Get())
  {
    FaserFieldSetup* emSetup = new FaserFieldSetup();
    fFieldSetup.Put(emSetup);
    G4AutoDelete::Register(emSetup);
  }
  G4bool allLocal = true;
  fLogicTracker->SetFieldManager(fFieldSetup.Get()->GetLocalFieldManager(),
				 allLocal);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTracker()
{
  //fTrackerFactory = new FaserSensorPlaneConstruction(this, G4String("tracker"));
  fLogicTrackerPlane = fTrackerFactory->Construct();

  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicTrackerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();
  G4double plane_sizeZ = 2*sPlane->GetZHalfLength();

  // tracker material
  //
  G4Material* tracker_mat = nist->FindOrBuildMaterial("G4_AIR");

  // air volume transverse size
  //
  G4double tracker_sizeX = plane_sizeX + 10.0*cm;
  G4double tracker_sizeY = plane_sizeY + 10.0*cm;

  // length of volume enclosing sensor planes (plus air gap behind)
  //
  // Must have at least 3 planes
  //
  tracker_sensorPlanes = std::max(tracker_sensorPlanes, 3);

  // Now user specified - check for sanity; the volume must be long enough
  // to hold all requested planes at the minumum possible spacing
  //
  detector_trackerLength = std::max(detector_trackerLength, 
				    (tracker_sensorPlanes - 1) * detector_planePitch + 
				    tracker_sensorPlanes * plane_sizeZ);

  // solid to hold sensor planes (and tracker magnetic field)
  //
  G4Box* solidTracker =    
    new G4Box("Tracker",                    //its name
        0.5*tracker_sizeX, 0.5*tracker_sizeY, 0.5*detector_trackerLength); //its size
      

  fLogicTracker =                         
    new G4LogicalVolume(solidTracker,            //its solid
                        tracker_mat,             //its material
                        "Tracker");              //its name

  // in the optimized setup, we have to figure out where to place the planes
  int nEndPlanes = 0;  // this is the number at each end
  int nCentralPlanes = 0;
  switch (tracker_sensorPlanes % 4)
  {
  case 0:
    nEndPlanes = tracker_sensorPlanes/4;
    nCentralPlanes = tracker_sensorPlanes/2;
    break;
  case 1:
    nCentralPlanes = (tracker_sensorPlanes+1)/2;
    nEndPlanes = (tracker_sensorPlanes - nCentralPlanes)/2;
    break;
  case 2:
    // choose 2-2-2 over 1-4-1 even though they are theoretically equivalent
    nCentralPlanes = tracker_sensorPlanes/2 - 1;
    nEndPlanes = (tracker_sensorPlanes - nCentralPlanes)/2;
    break;
  case 3:
    nCentralPlanes = (tracker_sensorPlanes - 1)/2;
    nEndPlanes = (tracker_sensorPlanes - nCentralPlanes)/2;
    break;
  default:
    G4cout << "Impossible condition reached" << G4endl;
    throw;
  }

  // Do the front end planes
  //
  G4double firstPlaneZ = -detector_trackerLength/2 + plane_sizeZ/2;
  for (G4int i = 0; i < nEndPlanes; i++)
  {
      new G4PVPlacement(0,
			G4ThreeVector(0, 0, firstPlaneZ + i*detector_planePitch),
			fLogicTrackerPlane,
			"Plane_PV",
			fLogicTracker,
			false,
			i,
			checkOverlaps);
  }
  
  // Central planes are evenly distributed around z = 0
  //
  firstPlaneZ = -(nCentralPlanes - 1) * (plane_sizeZ + detector_planePitch)/2.0;
  for (G4int i = 0; i < nCentralPlanes; i++)
  {
      new G4PVPlacement(0,
			G4ThreeVector(0, 0, firstPlaneZ + i*detector_planePitch),
			fLogicTrackerPlane,
			"Plane_PV",
			fLogicTracker,
			false,
			nEndPlanes + i,
			checkOverlaps);
  }

  // Downstream end planes are symmetrical to upstream
  //
  firstPlaneZ = detector_trackerLength/2 - plane_sizeZ/2 - (nEndPlanes - 1) * detector_planePitch;
  for (G4int i = 0; i < nEndPlanes; i++)
  {
      new G4PVPlacement(0,
			G4ThreeVector(0, 0, firstPlaneZ + i*detector_planePitch),
			fLogicTrackerPlane,
			"Plane_PV",
			fLogicTracker,
			false,
			nEndPlanes + nCentralPlanes + i,
			checkOverlaps);
  }
  // define a region encompassing the tracker
  //
  fLogicTracker->SetRegion(fRegTracker);
  fRegTracker->AddRootLogicalVolume(fLogicTracker);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructDecayVolume()
{
  // material for air volume
  G4Material* decay_mat = nist->FindOrBuildMaterial("G4_AIR");

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double decayVolumeLength = std::max(detector_decayVolumeLength, detector_planePitch);
  G4double decay_sizeZ = decayVolumeLength;

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double tracker_sizeX = 2*sTracker->GetXHalfLength();
  G4double tracker_sizeY = 2*sTracker->GetYHalfLength();

  G4double decay_sizeX = tracker_sizeX;
  G4double decay_sizeY = tracker_sizeY;

  G4Box* solidEnv =    
    new G4Box("DecayVolume",                 //its name
        0.5*decay_sizeX, 0.5*decay_sizeY, 0.5*decay_sizeZ); //its size
      
  fLogicDecayVolume =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        decay_mat,           //its material
                        "DecayVolume");      //its name

  fLogicDecayVolume->SetRegion(fRegTracker);
  fRegTracker->AddRootLogicalVolume(fLogicDecayVolume);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructCalorimeter()
{
  // material for air volume
  G4Material* calorimeter_mat = nist->FindOrBuildMaterial("G4_AIR");

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double calorimeterLength = std::max(detector_calorimeterLength, detector_planePitch);
  G4double calorimeter_sizeZ = calorimeterLength;

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double tracker_sizeX = 2*sTracker->GetXHalfLength();
  G4double tracker_sizeY = 2*sTracker->GetYHalfLength();

  G4double calorimeter_sizeX = tracker_sizeX;
  G4double calorimeter_sizeY = tracker_sizeY;

  G4Box* solidCalorimeter =    
    new G4Box("Calorimeter",                 //its name
        0.5*calorimeter_sizeX, 0.5*calorimeter_sizeY, 0.5*calorimeter_sizeZ); //its size
      
  fLogicCalorimeter =                         
    new G4LogicalVolume(solidCalorimeter,            //its solid
                        calorimeter_mat,           //its material
                        "Calorimeter");      //its name

  fLogicCalorimeter->SetRegion(fRegCalorimeter);
  fRegCalorimeter->AddRootLogicalVolume(fLogicCalorimeter);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructAbsorberPlane()
{
  G4Material* absorber_mat = nist->FindOrBuildMaterial("G4_W"); // Tungsten
  G4double tungstenRadLen = absorber_mat->GetRadlen();
  G4double absorberSizeZ = sampler_absorberX0 * tungstenRadLen;

  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicSamplerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();

  G4Box* solidAbsorber = new G4Box("Absorber", 
    0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*absorberSizeZ);
  fLogicAbsorber = 
    new G4LogicalVolume(solidAbsorber,
      absorber_mat, "Absorber");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructSampler()
{
  fLogicSamplerPlane = fSamplerFactory->Construct();

  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicSamplerPlane->GetSolid());
  G4double plane_sizeZ = 2*sPlane->GetZHalfLength();

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double sampler_sizeX = 2*sTracker->GetXHalfLength();
  G4double sampler_sizeY = 2*sTracker->GetYHalfLength();

  ConstructAbsorberPlane();

  const G4Box* sAbs = dynamic_cast<const G4Box*>(fLogicAbsorber->GetSolid());
  G4double absorber_sizeZ = 2*sAbs->GetZHalfLength();

  // just empty air for now
  G4Material* sampler_mat = nist->FindOrBuildMaterial("G4_AIR");

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double sampler_sizeZ = std::max(detector_samplerLength, sampler_sensorPlanes * (absorber_sizeZ + plane_sizeZ));

  // const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  // G4double tracker_sizeX = 2*sTracker->GetXHalfLength();
  // G4double tracker_sizeY = 2*sTracker->GetYHalfLength();

  G4Box* solidSampler =    
    new G4Box("Sampler",                 //its name
        0.5*sampler_sizeX, 0.5*sampler_sizeY, 0.5*sampler_sizeZ); //its size
      
  fLogicSampler =                         
    new G4LogicalVolume(solidSampler,    //its solid
                        sampler_mat,     //its material
                        "Sampler");      //its name

  G4double firstAbsorberZ = -0.5 * (sampler_sizeZ - absorber_sizeZ) + plane_sizeZ + (sampler_sensorPlanes - 1) * (plane_sizeZ + absorber_sizeZ);
  G4double firstPlaneZ = firstAbsorberZ + 0.5 * (absorber_sizeZ + plane_sizeZ);
  for (int i = 0; i < sampler_sensorPlanes; i++)
  {
      new G4PVPlacement(0,
      G4ThreeVector(0, 0, firstAbsorberZ + i * (plane_sizeZ + absorber_sizeZ)),
      fLogicAbsorber,
      "SamplerAbsorber_PV",
      fLogicSampler,
      false,
      i,
      checkOverlaps);

      new G4PVPlacement(0,
			G4ThreeVector(0, 0, firstPlaneZ + i * (plane_sizeZ + absorber_sizeZ)) ,
			fLogicSamplerPlane,
			"SamplerPlane_PV",
			fLogicSampler,
			false,
			i,
			checkOverlaps);
  }

  fLogicSampler->SetRegion(fRegCalorimeter);
  fRegCalorimeter->AddRootLogicalVolume(fLogicSampler);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* FaserDetectorConstruction::Construct()
{  
  // Get nist material manager
  //
  nist = G4NistManager::Instance();
  
  fRegTracker = new G4Region("Tracker");
  fRegCalorimeter = new G4Region("Calorimeter");

  // construct the tracker (and everything inside)
  ConstructTracker();

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double tracker_sizeZ = 2*sTracker->GetZHalfLength();

  // construct the decay volume
  ConstructDecayVolume();

  const G4Box* sDecay = dynamic_cast<const G4Box*>(fLogicDecayVolume->GetSolid());
  G4double decay_sizeZ = 2*sDecay->GetZHalfLength();

  // construct the pre-sampler
  ConstructSampler();

  const G4Box* sSampler = dynamic_cast<const G4Box*>(fLogicSampler->GetSolid());
  G4double sampler_sizeZ = 2*sSampler->GetZHalfLength();

  // construct the calorimeter
  ConstructCalorimeter();
  const G4Box* sCalorimeter = dynamic_cast<const G4Box*>(fLogicCalorimeter->GetSolid());
  G4double calorimeter_sizeZ = 2*sCalorimeter->GetZHalfLength();

  // world volume size
  // want front surface of first plane at z = 0 in world system
  //
  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicTrackerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();

  G4double world_sizeX = plane_sizeX + 2.0*m, world_sizeY = plane_sizeY + 2.0*m;
  G4double world_sizeZ = 2 * std::max(decay_sizeZ, tracker_sizeZ + sampler_sizeZ + calorimeter_sizeZ) + 2.0*m;

  // Define a region for the air volume inside the rock
  // we will abort tracking of particles that enter the surrounding default region
  //
  // fRegAir = new G4Region("Air");
  // fLogicTracker->SetRegion(fRegAir);
  // fRegAir->AddRootLogicalVolume(fLogicTracker);
  // fLogicDecayVolume->SetRegion(fRegAir);
  // fRegAir->AddRootLogicalVolume(fLogicDecayVolume);
  // fLogicSampler->SetRegion(fRegAir);
  // fRegAir->AddRootLogicalVolume(fLogicSampler);
  // fLogicCalorimeter->SetRegion(fRegAir);
  // fRegAir->AddRootLogicalVolume(fLogicCalorimeter);

  //     
  // World
  //
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  // Concrete results in massive showers as tracks exit the air volume, very slow to display
  //G4Material* world_mat = nist->FindOrBuildMaterial("G4_CONCRETE");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  // place the tracker volume inside
  //
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, +tracker_sizeZ/2 ),
                    fLogicTracker,           //its logical volume
                    "Tracker_PV",            //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // place the sampler volume inside
  //
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, tracker_sizeZ + sampler_sizeZ/2 ),
                    fLogicSampler,           //its logical volume
                    "Sampler_PV",            //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // place the calorimeter volume inside
  //
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, tracker_sizeZ + sampler_sizeZ + calorimeter_sizeZ/2 ),
                    fLogicCalorimeter,       //its logical volume
                    "Calorimeter_PV",            //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  // place the decay volume inside
  //                                   
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, -decay_sizeZ/2 ),
                    fLogicDecayVolume,       //its logical volume
                    "Decay_PV",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
