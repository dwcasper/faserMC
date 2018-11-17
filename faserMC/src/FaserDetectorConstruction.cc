// adapted from Geant4 example

#include "FaserDetectorConstruction.hh"
#include "FaserSensorPlaneConstruction.hh"
#include "FaserGeometryMessenger.hh"
#include "FaserTrackerGeometry.hh"
#include "FaserSensorSD.hh"
#include "FaserCaloSD.hh"
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
  : G4VUserDetectorConstruction(),
    fGeoConfig(GeometryConfig::SCT),
    fGeometryMessenger(new FaserGeometryMessenger(this)),
    //fTrackerGeo(new FaserTrackerGeometry),
    fLogicTracker(nullptr), fLogicTrackerPlane(nullptr), 
    fLogicSamplerPlane(nullptr), fLogicCaloModule(nullptr), fLogicCaloTower(nullptr),
    sensor_readoutStrips(default_sensor_readoutStrips),
    sensor_stripPitch(default_sensor_stripPitch),
    sensor_stripLength(default_sensor_stripLength),
    sensor_gap(default_sensor_gap),
    sensor_sizeZ(default_sensor_sizeZ),
    sensor_stereoAngle(default_sensor_stereoAngle),
    support_sizeZ(default_support_sizeZ),
    tracker_sensorPlanes(default_tracker_sensorPlanes),
    sampler_sensorPlanes(default_sampler_sensorPlanes),
    sampler_absorberC(default_sampler_absorberC),
    sampler_absorberCu(default_sampler_absorberCu),
    sampler_absorberW(default_sampler_absorberW),
    calo_planes(default_calo_planes),
    calo_towers(default_calo_towers),
    calo_modules(default_calo_modules),
    calo_scintThickness(default_calo_scintThickness),
    calo_absorbThickness(default_calo_absorbThickness),
    calo_tyvekThickness(default_calo_tyvekThickness),
    calo_planeXY(default_calo_planeXY),
    calo_moduleXY(default_calo_moduleXY),
    detector_samplerLength(default_detector_samplerLength),
    detector_planePitch(default_detector_planePitch),
    detector_decayVolumeLength(default_detector_decayVolumeLength),
    fSamplerRotation(nullptr),
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
  if (fSamplerRotation != nullptr) delete fSamplerRotation;
}

const G4LogicalVolume* FaserDetectorConstruction::GetTrackerStrip() const
{
  return fTrackerFactory->GetStrip();
}
const G4LogicalVolume* FaserDetectorConstruction::GetSamplerStrip() const
{
  return fSamplerFactory->GetStrip();
}
const G4LogicalVolume* FaserDetectorConstruction::GetCaloTower() const
{
  return fLogicCaloTower;
}

void FaserDetectorConstruction::ConstructSDandField()
{
  G4String trackerSDName = "Tracker";
  FaserSensorSD* trackerSD = new FaserSensorSD(trackerSDName, 
						  "FaserTrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(trackerSD);
  SetSensitiveDetector( "trackerStrip", trackerSD, true );

  G4String samplerSDName = "Sampler";
  FaserSensorSD* samplerSD = new FaserSensorSD(samplerSDName, 
						  "FaserSamplerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(samplerSD);
  SetSensitiveDetector( "samplerStrip", samplerSD, true );

  G4String caloSDName = "Calorimeter";
  FaserCaloSD* caloSD = new FaserCaloSD(caloSDName, 
						  "FaserCaloHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(caloSD);
  SetSensitiveDetector( "caloTower", caloSD, true );


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
  fLogicDecayVolume->SetFieldManager(fFieldSetup.Get()->GetLocalFieldManager(), 
    allLocal);
}

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
    // kludge to do 3-3-3 when we have nine planes
    if (tracker_sensorPlanes == 9)
    {
      nCentralPlanes = 3;
      nEndPlanes = 3;
    }
    else
    {
      nCentralPlanes = (tracker_sensorPlanes+1)/2;
      nEndPlanes = (tracker_sensorPlanes - nCentralPlanes)/2;
    }
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

      double zPlane = 996.01 + firstPlaneZ + i*detector_planePitch;
      G4cout << "Updating tracking geometry: planeZ[" << i << "] = " << zPlane << "\n";
      // fTrackerGeo->planeZ.push_back(zPlane);
      // fTrackerGeo->planeIndices_front.push_back(i);
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

      double zPlane = 996.01 + firstPlaneZ + i*detector_planePitch;
      G4cout << "Updating tracking geometry: planeZ[" << nEndPlanes + i << "] = " << zPlane << "\n";
      // fTrackerGeo->planeZ.push_back(zPlane);
      // fTrackerGeo->planeIndices_central.push_back(nEndPlanes + i);
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

      double zPlane = 996.01 + firstPlaneZ + i*detector_planePitch;
      G4cout << "Updating tracking geometry: planeZ[" << nEndPlanes + nCentralPlanes + i << "] = " << zPlane << "\n";
      // fTrackerGeo->planeZ.push_back(zPlane);
      // fTrackerGeo->planeIndices_end.push_back(nEndPlanes + nCentralPlanes + i);
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

void FaserDetectorConstruction::ConstructCalorimeterModule()
{
  G4Material* caloModule_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4double calo_sizeZ = (calo_planes - 1) * calo_absorbThickness + 
    calo_planes * (calo_scintThickness + 2 * calo_tyvekThickness);

  G4Box* solidCaloModule = new G4Box("CaloModule", 0.5 * calo_moduleXY, 0.5 * calo_moduleXY, 0.5 * calo_sizeZ);
  fLogicCaloModule = new G4LogicalVolume(solidCaloModule, caloModule_mat, "caloModule");

  G4Material* tyvek_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Box* solidTyvek = new G4Box("TyvekSheet", 0.5 * calo_moduleXY, 0.5 * calo_moduleXY, 0.5 * calo_tyvekThickness);
  G4LogicalVolume* logicTyvek = new G4LogicalVolume(solidTyvek, tyvek_mat, "tyvekSheet");

  G4Material* scint_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Box* solidScint = new G4Box("CaloScintPlane", 0.5 * calo_moduleXY, 0.5 * calo_moduleXY, 0.5 * calo_scintThickness);
  G4LogicalVolume* logicScint = new G4LogicalVolume(solidScint, scint_mat, "caloPlane");
  G4int caloTowersAcross = (calo_towers == 9 ? 3 : (calo_towers == 4 ? 2 : 1) );
  G4double caloTowerXY = calo_moduleXY / caloTowersAcross;
  G4Box* solidTower = new G4Box("CaloScintTower", 0.5 * caloTowerXY, 
    0.5 * caloTowerXY, 0.5 * calo_scintThickness);
  fLogicCaloTower = new G4LogicalVolume(solidTower, scint_mat, "caloTower");
  
  G4int towerCount = 0;
  for (int ix = 0; ix < caloTowersAcross; ix++)
  {
    G4double x = ((1 - caloTowersAcross)/2.0 + ix) * caloTowerXY;
    for (int iy = 0; iy < caloTowersAcross; iy++)
    {
      G4double y = ((1 - caloTowersAcross)/2.0 + iy) * caloTowerXY;
      new G4PVPlacement(nullptr, G4ThreeVector(x, y, 0.0), fLogicCaloTower, "PV_CaloTower", logicScint, false, towerCount, checkOverlaps);
      towerCount++;
    }
  }

  G4Material* abs_mat = nist->FindOrBuildMaterial("G4_W");
  G4Box* solidAbsorb = new G4Box("CaloAbsorbPlane", 0.5* calo_moduleXY, 0.5 * calo_moduleXY, 0.5 * calo_absorbThickness);
  G4LogicalVolume* logicAbsorb = new G4LogicalVolume(solidAbsorb, abs_mat, "caloAbsorber");

  G4double startZ = - calo_sizeZ/2.0;
  G4int scintCount = 0;
  G4int tyvekCount = 0;
  G4int absCount = 0;
  G4double currentZ = startZ;
  do
  {
    currentZ = startZ + tyvekCount * calo_tyvekThickness + scintCount * calo_scintThickness + absCount * calo_absorbThickness + calo_tyvekThickness / 2.0;
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, currentZ), logicTyvek, "PV_Tyvek", fLogicCaloModule, false, tyvekCount, checkOverlaps);
    tyvekCount++;
    currentZ = startZ + tyvekCount * calo_tyvekThickness + scintCount * calo_scintThickness + absCount * calo_absorbThickness + calo_scintThickness / 2.0;
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, currentZ), logicScint, "PV_CaloScint", fLogicCaloModule, false, scintCount, checkOverlaps);
    scintCount++;
    currentZ = startZ + tyvekCount * calo_tyvekThickness + scintCount * calo_scintThickness + absCount * calo_absorbThickness + calo_tyvekThickness / 2.0;
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, currentZ), logicTyvek, "PV_Tyvek", fLogicCaloModule, false, tyvekCount, checkOverlaps);
    tyvekCount++;
    if (scintCount >= calo_planes) break;
    currentZ = startZ + tyvekCount * calo_tyvekThickness + scintCount * calo_scintThickness + absCount * calo_absorbThickness + calo_absorbThickness / 2.0;
    new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, currentZ), logicAbsorb, "PV_CaloAbsorb", fLogicCaloModule, false, absCount, checkOverlaps);
    absCount++;
  } while (scintCount < calo_planes);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructCalorimeter()
{
  // material for air volume
  G4Material* calorimeter_mat = nist->FindOrBuildMaterial("G4_AIR");

  ConstructCalorimeterModule();

  G4int caloModulesAcross = (calo_modules == 9 ? 3 : (calo_modules == 4 ? 2 : (calo_modules == 1 ? 1 : 0)));

  G4double calorimeter_sizeXY = calo_moduleXY * caloModulesAcross;
  
  const G4Box* sCaloModule = dynamic_cast<const G4Box*>(fLogicCaloModule->GetSolid());
  G4double calorimeterModule_sizeZ = 2 * sCaloModule->GetZHalfLength();

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double calorimeterLength = std::max(detector_calorimeterLength, std::max(detector_planePitch, calorimeterModule_sizeZ));
  G4double calorimeter_sizeZ = calorimeterLength;

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double tracker_sizeX = 2*sTracker->GetXHalfLength();
  G4double tracker_sizeY = 2*sTracker->GetYHalfLength();

  G4double calorimeter_sizeX = std::max(tracker_sizeX, calorimeter_sizeXY);
  G4double calorimeter_sizeY = std::max(tracker_sizeY, calorimeter_sizeXY);

  G4Box* solidCalorimeter =    
    new G4Box("Calorimeter",                 //its name
        0.5*calorimeter_sizeX, 0.5*calorimeter_sizeY, 0.5*calorimeter_sizeZ); //its size
      
  fLogicCalorimeter =                         
    new G4LogicalVolume(solidCalorimeter,            //its solid
                        calorimeter_mat,           //its material
                        "Calorimeter");      //its name

  G4int caloModuleIndex = 0;
  G4double caloModule_initialXY = - ((caloModulesAcross - 1) * calo_moduleXY)/2.0;
  G4double caloModuleZ = - (calorimeter_sizeZ - calorimeterModule_sizeZ)/2.0;  // fix this to put calomodule at forward end 
  for (int ix = 0; ix < caloModulesAcross; ix++)
  {
    G4double caloModuleX = caloModule_initialXY + ix * calo_moduleXY;
    for (int iy = 0; iy < caloModulesAcross; iy++)
    {
      G4double caloModuleY = caloModule_initialXY + iy * calo_moduleXY;

      new G4PVPlacement(0,
			G4ThreeVector(caloModuleX, caloModuleY, caloModuleZ),
			fLogicCaloModule,
			"CaloModule_PV",
			fLogicCalorimeter,
			false,
			caloModuleIndex,
			checkOverlaps);

      caloModuleIndex++;
    }
  }
  fLogicCalorimeter->SetRegion(fRegCalorimeter);
  fRegCalorimeter->AddRootLogicalVolume(fLogicCalorimeter);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructAbsorberPlane()
{
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* absorber_C = nist->FindOrBuildMaterial("G4_GRAPHITE"); // Graphite
  G4Material* absorber_Cu = nist->FindOrBuildMaterial("G4_Cu"); // Copper
  G4Material* absorber_W = nist->FindOrBuildMaterial("G4_W"); // Tungsten
  G4double tungstenThickness = sampler_absorberW * absorber_W->GetRadlen();
  G4double copperThickness = sampler_absorberCu * absorber_Cu->GetRadlen();
  G4double graphiteThickness = sampler_absorberC * absorber_C->GetRadlen();
  G4double absorberSizeZ = tungstenThickness + copperThickness + graphiteThickness;

  G4cout << "Presampler absorber thickness (cm) - W : " << tungstenThickness/cm << ", Cu : " << copperThickness/cm << ", Graphite: " << graphiteThickness/cm << G4endl;

  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicSamplerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();

  G4Box* solidAbsorber = new G4Box("Absorber", 
    0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*absorberSizeZ);
  fLogicAbsorber = 
    new G4LogicalVolume(solidAbsorber,
      air_mat, "Absorber");

  if (sampler_absorberC > 0)
  {
    G4Box* solidGraphite = new G4Box("samplerGraphiteSolid", 
      0.5 * plane_sizeX, 0.5 * plane_sizeY, 0.5 * graphiteThickness);
    G4LogicalVolume* logicGraphite = new G4LogicalVolume(solidGraphite, absorber_C, "samplerGraphiteAbsorber");
    new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -(absorberSizeZ - graphiteThickness)/2.0), 
                      logicGraphite, "PV_SamplerGraphite", fLogicAbsorber, false, checkOverlaps);
  }

  if (sampler_absorberCu > 0)
  {
    G4Box* solidCopper = new G4Box("samplerCopperSolid", 
      0.5 * plane_sizeX, 0.5 * plane_sizeY, 0.5 * copperThickness);
    G4LogicalVolume* logicCopper = new G4LogicalVolume(solidCopper, absorber_Cu, "samplerCopperAbsorber");
    new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -(absorberSizeZ - copperThickness)/2.0 + graphiteThickness), 
                      logicCopper, "PV_SamplerCopper", fLogicAbsorber, false, checkOverlaps);
  }

  if (sampler_absorberW > 0)
  {
    G4Box* solidTungsten = new G4Box("samplerTungstenSolid", 
      0.5 * plane_sizeX, 0.5 * plane_sizeY, 0.5 * tungstenThickness);
    G4LogicalVolume* logicTungsten = new G4LogicalVolume(solidTungsten, absorber_W, "samplerTungstenAbsorber");
    new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -(absorberSizeZ - tungstenThickness)/2.0 + graphiteThickness + copperThickness), 
                      logicTungsten, "PV_SamplerTungsten", fLogicAbsorber, false, checkOverlaps);
  }
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
  G4double sampler_sizeZ = std::max(detector_samplerLength, sampler_sensorPlanes * (absorber_sizeZ + plane_sizeZ + detector_planePitch) + detector_planePitch);

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

  fSamplerRotation = new G4RotationMatrix();
  fSamplerRotation->rotateZ(CLHEP::pi/2);

  G4double usableZ = sampler_sizeZ - 2 * (detector_planePitch);
  G4double firstMidpoint = -0.5*(usableZ - (plane_sizeZ + absorber_sizeZ));

  for (int i = 0; i < sampler_sensorPlanes; i++)
  {
      G4double thisMidpoint = firstMidpoint + i * (plane_sizeZ + absorber_sizeZ + detector_planePitch);
      G4double thisAbsorberZ = thisMidpoint - plane_sizeZ/2.0;
      G4double thisPlaneZ = thisMidpoint + absorber_sizeZ/2.0;
      G4RotationMatrix* theRotation = ( i%2 > 0 ? fSamplerRotation : nullptr);

      new G4PVPlacement(theRotation,
      G4ThreeVector(0, 0, thisAbsorberZ),
      fLogicAbsorber,
      "SamplerAbsorber_PV",
      fLogicSampler,
      false,
      i,
      checkOverlaps);

      new G4PVPlacement(theRotation,
			G4ThreeVector(0, 0, thisPlaneZ) ,
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
  G4double calorimeter_sizeX = 2*sCalorimeter->GetXHalfLength();
  G4double calorimeter_sizeY = 2*sCalorimeter->GetYHalfLength();
  G4double calorimeter_sizeZ = 2*sCalorimeter->GetZHalfLength();

  // world volume size
  // want front surface of first plane at z = 0 in world system
  //
  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicTrackerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();

  G4double world_sizeX = std::max(plane_sizeX, calorimeter_sizeX) + 1.0*m;
  G4double world_sizeY = std::max(plane_sizeY, calorimeter_sizeY) + 1.0*m;
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
  fCaloMinZ = tracker_sizeZ + sampler_sizeZ;
  fCaloMaxZ = fCaloMinZ + calorimeter_sizeZ;

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

  //fTrackerGeo->WriteToFile("faserTrackerGeo.root");

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
