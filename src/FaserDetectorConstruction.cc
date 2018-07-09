// adapted from Geant4 example

#include "FaserDetectorConstruction.hh"
#include "FaserGeometryMessenger.hh"
#include "FaserSensorSD.hh"
//#include "FaserSamplerSD.hh"
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
    fLogicTracker(nullptr), fLogicTrackerPlane(nullptr), fLogicTrackerModule(nullptr), 
    fLogicTrackerSensor(nullptr), fLogicTrackerRow(nullptr), fLogicTrackerStrip(nullptr),
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
  fStereoPlus(nullptr), fStereoMinus(nullptr), fOverlapAngle(nullptr),
  checkOverlaps(true), nist(nullptr), fRegTracker(nullptr), fRegAir(nullptr)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserDetectorConstruction::~FaserDetectorConstruction()
{ 
  if (fGeometryMessenger) delete fGeometryMessenger;
  if (fStereoPlus) delete fStereoPlus;
  if (fStereoMinus) delete fStereoMinus;
  if (fOverlapAngle) delete fOverlapAngle;
}

void FaserDetectorConstruction::ConstructSDandField()
{
  G4String sensorSDName = "Faser/SensorSD";
  FaserSensorSD* aSensorSD = new FaserSensorSD(sensorSDName, 
						  "FaserSensorHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aSensorSD);
  SetSensitiveDetector( "Strip", aSensorSD, true );

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

void FaserDetectorConstruction::ConstructTrackerSensor()
{
  // sensor dimensions
  //
  // In x, there are field-shaping strips at each end, and we assume a border on each edge of sensor_gap/2
  //
  G4double sensor_sizeX = ( sensor_readoutStrips + 2 ) * sensor_stripPitch + sensor_gap;

  // In y, there are two rows of strips, separated by a gap, and we assume border on each edge of sensor_gap/2
  //
  G4double sensor_sizeY = ( sensor_stripLength * 2) + (sensor_gap * 2);
  
  G4cout << "Sensor dimensions: ( " << sensor_sizeX/mm << " mm, " << sensor_sizeY/mm << " mm, "
	 << sensor_sizeZ/mm << " mm )" << G4endl; 

  // sensor volume
  //
  G4Material* sensor_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box* solidSensor =    
    new G4Box("Sensor",                       //its name
       0.5*sensor_sizeX, 0.5*sensor_sizeY, 0.5*sensor_sizeZ);     //its size
      
  fLogicTrackerSensor =                         
    new G4LogicalVolume(solidSensor,          //its solid
                        sensor_mat,           //its material
                        "Sensor");            //its name
  
  // strip row - this contains only the active volume of the strips
  //
  G4Box* solidRow = 
    new G4Box("Row",
	      0.5*(sensor_readoutStrips * sensor_stripPitch), 0.5*sensor_stripLength, 0.5*sensor_sizeZ);
  fLogicTrackerRow =
    new G4LogicalVolume(solidRow,
			sensor_mat,
			"Row");

  // place the two rows inside the sensor
  new G4PVPlacement(0,
		    G4ThreeVector(0, -0.5*(sensor_stripLength + sensor_gap), 0),
		    fLogicTrackerRow,
		    "Row_PV",
		    fLogicTrackerSensor,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(0,
		    G4ThreeVector(0, +0.5*(sensor_stripLength + sensor_gap), 0),
		    fLogicTrackerRow,
		    "Row_PV",
		    fLogicTrackerSensor,
		    false,
		    1,
		    checkOverlaps);
  
  // a single strip; this will be replicated
  //	      
  G4Box* solidStrip =
    new G4Box("Strip",
	      0.5*sensor_stripPitch, 0.5*sensor_stripLength, 0.5*sensor_sizeZ);
  fLogicTrackerStrip = 
    new G4LogicalVolume(solidStrip,
		       sensor_mat,
		       "Strip");

  // place the requested number of strips inside the row
  //
  new G4PVReplica("Strip_PV",
		  fLogicTrackerStrip,
		  fLogicTrackerRow,
		  kXAxis,
		  sensor_readoutStrips,
		  sensor_stripPitch,
		  0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTrackerModule()
{
  // Construct a single Si sensor region
  //
  ConstructTrackerSensor();

  const G4Box* sSensor = dynamic_cast<const G4Box*>(fLogicTrackerSensor->GetSolid());
  G4double sensor_sizeX = 2*sSensor->GetXHalfLength();
  G4double sensor_sizeY = 2*sSensor->GetYHalfLength();

  // sensor stereo angle (+/-)
  //
  G4cout << "Stereo angle: " << sensor_stereoAngle/mrad << " mrad" << G4endl;

  // create rotation matrices for both senses of rotation
  // we have to hold these pointers (and not change the objects)
  // until the end of the job

  // this combination of rotations, rather than the more obvious single rotation
  // is necessary for hit/digit drawing of strips to draw them in the right place
  // I don't really understand it...

  fStereoPlus = new G4RotationMatrix;
  fStereoPlus->rotateY(CLHEP::pi);
  fStereoPlus->rotateZ(-sensor_stereoAngle);

  fStereoMinus = new G4RotationMatrix;
  fStereoMinus->rotateY(CLHEP::pi);
  fStereoMinus->rotateZ(sensor_stereoAngle);

  // support dimensions - assume a 2:1 rectangular shape
  //
  G4double support_sizeX = sensor_sizeX * cos(sensor_stereoAngle) + sensor_sizeY * sin(sensor_stereoAngle);
  G4double support_sizeY = sensor_sizeY * cos(sensor_stereoAngle) + sensor_sizeX * sin(sensor_stereoAngle) 
    + sensor_sizeY/cos(sensor_stereoAngle);
  G4cout << "Support dimensions: " << support_sizeX/mm << " mm (X), "
	 << support_sizeY/mm << " mm (Y), " << support_sizeZ/mm << " mm (Z)" << G4endl;

  //support volume
  //
  G4Material* support_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");
  G4Box* solidSupport =
    new G4Box("Support", 0.5*support_sizeX, 0.5*support_sizeY, 0.5*support_sizeZ);

  G4LogicalVolume* logicSupport =
    new G4LogicalVolume(solidSupport,
			support_mat,
			"Support");

  // module dimensions
  //
  G4double module_sizeX = support_sizeX;
  G4double module_sizeY = support_sizeY;
  G4double module_sizeZ = 2 * sensor_sizeZ + support_sizeZ;

  // module volume
  //
  G4Material* module_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidModule = 
    new G4Box("Module", 0.5*module_sizeX, 0.5*module_sizeY, 0.5*module_sizeZ);

  fLogicTrackerModule = 
    new G4LogicalVolume(solidModule,
			module_mat,
			"Module");

  // place support inside module volume
  //
  new G4PVPlacement(0,                     //no rotation
                    G4ThreeVector(),       //at (0,0,0)
                    logicSupport,          //its logical volume
                    "Support_PV",          //its name
                    fLogicTrackerModule,   //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
  
  // place sensors inside module volume
  // the same logical volume is re-used, but it is translated and rotated four different ways
  //
  new G4PVPlacement(fStereoMinus,
                    G4ThreeVector(0.0, 0.5*sensor_sizeY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
	            fLogicTrackerSensor,
	            "Sensor_PV",
	            fLogicTrackerModule,
	            false,
	            0,
	            checkOverlaps);

  new G4PVPlacement(fStereoPlus,
		    G4ThreeVector(0.0, 0.5*sensor_sizeY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    fLogicTrackerSensor,
		    "Sensor_PV",
		    fLogicTrackerModule,
		    false,
		    1,
		    checkOverlaps);

  new G4PVPlacement(fStereoMinus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
		    fLogicTrackerSensor,
		    "Sensor_PV",
		    fLogicTrackerModule,
		    false,
		    2,
		    checkOverlaps);

  new G4PVPlacement(fStereoPlus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    fLogicTrackerSensor,
		    "Sensor_PV",
		    fLogicTrackerModule,
		    false,
		    3,
		    checkOverlaps);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTrackerPlane()
{

  // Construct a tracker module (two of them overlap to form a plane)
  ConstructTrackerModule();

  const G4Box* sModule = dynamic_cast<const G4Box*>(fLogicTrackerModule->GetSolid());
  G4double module_sizeX = 2*sModule->GetXHalfLength();
  G4double module_sizeY = 2*sModule->GetYHalfLength();
  G4double module_sizeZ = 2*sModule->GetZHalfLength();

  // effective half-width of the wafer due to stereo rotation
  //
  const G4Box* sSensor = dynamic_cast<const G4Box*>(fLogicTrackerSensor->GetSolid());
  G4double sensor_sizeX = 2*sSensor->GetXHalfLength();
  G4double wPrime = (sensor_sizeX/2) / cos(sensor_stereoAngle);

  // overlap angle that will allow maximum x-separation without any gap
  //
  G4double overlapAngle = asin( module_sizeZ/wPrime )/2;
  
  // corresponding x separation of modules with this angle
  //
  G4double xOffset = wPrime * cos(overlapAngle);

  // Rotation matrix for module overlap - again, must preserve unchanged until end of job
  //
  fOverlapAngle = new G4RotationMatrix;
  fOverlapAngle->rotateY(overlapAngle);

  // work out the size of the plane box that will contain both modules
  //
  G4double plane_sizeX = 2 * xOffset + module_sizeX * cos(overlapAngle) + module_sizeZ * sin(overlapAngle);
  G4double plane_sizeY = module_sizeY;
  G4double plane_sizeZ = module_sizeX * sin(overlapAngle) + module_sizeZ * cos(overlapAngle);
  G4cout << "Plane dimensions: " << plane_sizeX/mm << " mm (X), "
	 << plane_sizeY/mm << " mm (Y), " << plane_sizeZ/mm << " mm (Z)" << G4endl;

  // plane volume
  //
  G4Material* plane_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidPlane =
    new G4Box("Plane", 0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*plane_sizeZ);

  fLogicTrackerPlane =
    new G4LogicalVolume(solidPlane,
                        plane_mat,
                        "Plane");

  // place modules inside plane
  //
  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(-xOffset, 0, 0),
		    fLogicTrackerModule,
		    "Module_PV",
		    fLogicTrackerPlane,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(+xOffset, 0, 0),
		    fLogicTrackerModule,
		    "Module_PV",
		    fLogicTrackerPlane,
		    false,
		    1,
 		    checkOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTrackerOld()
{

  // construct a two-sided sensor plane
  ConstructTrackerPlane();

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
  detector_trackerLength = tracker_sensorPlanes * detector_planePitch + 
    plane_sizeZ;

  // solid to hold sensor planes (and tracker magnetic field)
  //
  G4Box* solidTracker =    
    new G4Box("Tracker",                    //its name
        0.5*tracker_sizeX, 0.5*tracker_sizeY, 0.5*detector_trackerLength); //its size
      

  fLogicTracker =                         
    new G4LogicalVolume(solidTracker,            //its solid
                        tracker_mat,             //its material
                        "Tracker");              //its name

  G4double firstPlaneZ = -detector_trackerLength/2 + plane_sizeZ/2;

  for (G4int i = 0; i < tracker_sensorPlanes; i++)
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTracker()
{

  // construct a two-sided sensor plane
  ConstructTrackerPlane();

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
  fLogicTrackerPlane->SetRegion(fRegTracker);
  fRegTracker->AddRootLogicalVolume(fLogicTrackerPlane);

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
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructSamplerPlane()
{
  G4Material* absorber_mat = nist->FindOrBuildMaterial("G4_W"); // Tungsten
  G4double tungstenRadLen = absorber_mat->GetRadlen();
  G4double absorberSizeZ = sampler_absorberX0 * tungstenRadLen;

  G4Material* sampler_mat = nist->FindOrBuildMaterial("G4_AIR");

  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicTrackerPlane->GetSolid());
  G4double plane_sizeX = 2*sPlane->GetXHalfLength();
  G4double plane_sizeY = 2*sPlane->GetYHalfLength();
  G4double plane_sizeZ = 2*sPlane->GetZHalfLength();

  G4Box* solidAbsorber = new G4Box("Absorber", 
    0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*absorberSizeZ);
  fLogicAbsorber = 
    new G4LogicalVolume(solidAbsorber,
      absorber_mat, "Absorber");

  // G4Box* solidSampler = new G4Box("Sampler",
  //   0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*detector_samplerLength);
  // fLogicSampler = new G4LogicalVolume(solidSampler, 
  //   sampler_mat, "Sampler");
  
  G4Box* solidSamplerPlane = new G4Box("SamplerPlane", 
    0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*(absorberSizeZ + plane_sizeZ));

  fLogicSamplerPlane = new G4LogicalVolume(solidSamplerPlane, sampler_mat, "SamplerPlane");

  new G4PVPlacement(0,                       //no rotation
                  G4ThreeVector(0, 0, +0.5*absorberSizeZ ),
                  fLogicTrackerPlane,           //its logical volume
                  "Sampler_PV",            //its name
                  fLogicSamplerPlane,  //its mother  volume
                  false,                   //no boolean operation
                  0,                       //copy number
                  checkOverlaps);          //overlaps checking

  new G4PVPlacement(0,                       //no rotation
                  G4ThreeVector(0, 0, -0.5*plane_sizeZ ),
                  fLogicAbsorber,           //its logical volume
                  "Absorber_PV",            //its name
                  fLogicSamplerPlane,  //its mother  volume
                  false,                   //no boolean operation
                  0,                       //copy number
                  checkOverlaps);          //overlaps checking

  fLogicAbsorber->SetRegion(fRegCalorimeter);
  fRegCalorimeter->AddRootLogicalVolume(fLogicAbsorber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructSampler()
{
  
  ConstructSamplerPlane();
  
  const G4Box* sPlane = dynamic_cast<const G4Box*>(fLogicSamplerPlane->GetSolid());
  G4double plane_sizeZ = 2*sPlane->GetZHalfLength();
  G4double sampler_sizeX = 2*sPlane->GetXHalfLength();
  G4double sampler_sizeY = 2*sPlane->GetYHalfLength();

  
  // just empty air for now
  G4Material* calorimeter_mat = nist->FindOrBuildMaterial("G4_AIR");

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double sampler_sizeZ = std::max(detector_samplerLength, sampler_sensorPlanes * plane_sizeZ);

  // const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  // G4double tracker_sizeX = 2*sTracker->GetXHalfLength();
  // G4double tracker_sizeY = 2*sTracker->GetYHalfLength();

  G4Box* solidSampler =    
    new G4Box("Sampler",                 //its name
        0.5*sampler_sizeX, 0.5*sampler_sizeY, 0.5*sampler_sizeZ); //its size
      
  fLogicSampler =                         
    new G4LogicalVolume(solidSampler,    //its solid
                        calorimeter_mat,     //its material
                        "Sampler");      //its name

  G4double firstPlaneZ = -0.5 * (sampler_sizeZ - plane_sizeZ) + (sampler_sensorPlanes - 1) * plane_sizeZ;
  for (int i = 0; i < sampler_sensorPlanes; i++)
  {
      new G4PVPlacement(0,
			G4ThreeVector(0, 0, firstPlaneZ + i*plane_sizeZ),
			fLogicSamplerPlane,
			"CalorimeterPlane_PV",
			fLogicSampler,
			false,
			i + tracker_sensorPlanes,
			checkOverlaps);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* FaserDetectorConstruction::Construct()
{  
  // Get nist material manager
  //
  nist = G4NistManager::Instance();
  
  // construct the tracker (and everything inside)
  fRegTracker = new G4Region("Tracker");
  ConstructTracker();

  const G4Box* sTracker = dynamic_cast<const G4Box*>(fLogicTracker->GetSolid());
  G4double tracker_sizeZ = 2*sTracker->GetZHalfLength();

  // construct the decay volume
  ConstructDecayVolume();

  const G4Box* sDecay = dynamic_cast<const G4Box*>(fLogicDecayVolume->GetSolid());
  G4double decay_sizeZ = 2*sDecay->GetZHalfLength();

  fRegCalorimeter = new G4Region("Calorimeter");

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
  fRegAir = new G4Region("Air");
  fLogicTracker->SetRegion(fRegAir);
  fRegAir->AddRootLogicalVolume(fLogicTracker);
  fLogicDecayVolume->SetRegion(fRegAir);
  fRegAir->AddRootLogicalVolume(fLogicDecayVolume);
  fLogicSampler->SetRegion(fRegAir);
  fRegAir->AddRootLogicalVolume(fLogicSampler);
  fLogicCalorimeter->SetRegion(fRegAir);
  fRegAir->AddRootLogicalVolume(fLogicCalorimeter);

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
