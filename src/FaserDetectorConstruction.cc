// adapted from Geant4 example

#include "FaserDetectorConstruction.hh"
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
    fLogicTracker(nullptr), fLogicTrackerPlane(nullptr), fLogicTrackerModule(nullptr), fLogicTrackerSensor(nullptr),
    sensor_readoutStrips(default_sensor_readoutStrips),
    sensor_stripPitch(default_sensor_stripPitch),
    sensor_stripLength(default_sensor_stripLength),
    sensor_gap(default_sensor_gap),
    sensor_sizeZ(default_sensor_sizeZ),
    sensor_stereoAngle(default_sensor_stereoAngle),
    support_sizeZ(default_support_sizeZ),
    detector_sensorPlanes(default_detector_sensorPlanes),
    detector_planePitch(default_detector_planePitch),
    detector_decayVolumeLength(default_detector_decayVolumeLength),
  fStereoPlus(nullptr), fStereoMinus(nullptr), fOverlapAngle(nullptr),
  checkOverlaps(true), nist(nullptr), env_mat(nullptr), env_sizeX(0),
  env_sizeY(0), plane_sizeX(0), plane_sizeY(0), plane_sizeZ(0), 
  module_sizeX(0), module_sizeY(0), module_sizeZ(0), sensor_sizeX(0), sensor_sizeY(0),
  firstPlaneZ(0)
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
  sensor_sizeX = ( sensor_readoutStrips + 2 ) * sensor_stripPitch + sensor_gap;

  // In y, there are two rows of strips, separated by a gap, and we assume border on each edge of sensor_gap/2
  //
  sensor_sizeY = ( sensor_stripLength * 2) + (sensor_gap * 2);
  
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
  G4LogicalVolume* logicRow =
    new G4LogicalVolume(solidRow,
			sensor_mat,
			"Row");

  // place the two rows inside the sensor
  new G4PVPlacement(0,
		    G4ThreeVector(0, -0.5*(sensor_stripLength + sensor_gap), 0),
		    logicRow,
		    "Row_PV",
		    fLogicTrackerSensor,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(0,
		    G4ThreeVector(0, +0.5*(sensor_stripLength + sensor_gap), 0),
		    logicRow,
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
  G4LogicalVolume* logicStrip = 
    new G4LogicalVolume(solidStrip,
		       sensor_mat,
		       "Strip");

  // place the requested number of strips inside the row
  //
  new G4PVReplica("Strip_PV",
		  logicStrip,
		  logicRow,
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
  // sensor stereo angle (+/-)
  //
  G4cout << "Stereo angle: " << sensor_stereoAngle/mrad << " mrad" << G4endl;

  // create rotation matrices for both senses of rotation
  // we have to hold these pointers (and not change the objects)
  // until the end of the job
  fStereoPlus = new G4RotationMatrix;
  fStereoPlus->rotateZ(sensor_stereoAngle);

  fStereoMinus = new G4RotationMatrix;
  fStereoMinus->rotateZ(-sensor_stereoAngle);

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
  module_sizeX = support_sizeX;
  module_sizeY = support_sizeY;
  module_sizeZ = 2 * sensor_sizeZ + support_sizeZ;

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

  // effective half-width of the wafer due to stereo rotation
  //
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
  plane_sizeX = 2 * xOffset + module_sizeX * cos(overlapAngle) + module_sizeZ * sin(overlapAngle);
  plane_sizeY = module_sizeY;
  plane_sizeZ = module_sizeX * sin(overlapAngle) + module_sizeZ * cos(overlapAngle);
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

  // define a region encompassing the tracker plane(s)
  //
  G4Region* tracker = new G4Region("Tracker");
  fLogicTrackerPlane->SetRegion(tracker);
  tracker->AddRootLogicalVolume(fLogicTrackerPlane);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserDetectorConstruction::ConstructTracker()
{

  // construct a two-sided sensor plane
  ConstructTrackerPlane();

  // air volume (envelope) material transverse size
  //
  env_sizeX = plane_sizeX + 10.0*cm, env_sizeY = plane_sizeY + 10.0*cm;

  // length of volume enclosing sensor planes (plus air gap behind)
  //
  //G4double allPlanes_sizeZ = detector_sensorPlanes * detector_planePitch + 
  //  plane_sizeZ;
  detector_trackerLength = detector_sensorPlanes * detector_planePitch + 
    plane_sizeZ;

  // solid to hold sensor planes (and tracker magnetic field)
  //
  G4Box* solidTracker =    
    new G4Box("Tracker",                    //its name
        0.5*env_sizeX, 0.5*env_sizeY, 0.5*detector_trackerLength); //its size
      

  fLogicTracker =                         
    new G4LogicalVolume(solidTracker,            //its solid
                        env_mat,                 //its material
                        "Tracker");              //its name

  firstPlaneZ = -detector_trackerLength/2 + plane_sizeZ/2;

  for (G4int i = 0; i < detector_sensorPlanes; i++)
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

G4VPhysicalVolume* FaserDetectorConstruction::Construct()
{  

  // Get nist material manager
  //
  nist = G4NistManager::Instance();
  
  // material for air
  env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // construct the tracker (and everything inside)
  ConstructTracker();

  // the z length of the lab includes a gap decayVolumeLength before the first
  // sensor plane, and a gap of planePitch after the last one
  //
  G4double decayVolumeLength = std::max(detector_decayVolumeLength, detector_planePitch);
  G4double env_sizeZ = decayVolumeLength;

  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeX, 0.5*env_sizeY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  // world volume size
  // want front surface of first plane at z = 0 in world system
  //
  G4double world_sizeX = plane_sizeX + 2.0*m, world_sizeY = plane_sizeY + 2.0*m;
  G4double world_sizeZ = 2 * std::max(decayVolumeLength, detector_trackerLength) + 2.0*m;

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
                    G4ThreeVector(0, 0, -(firstPlaneZ - plane_sizeZ/2) ),
                    fLogicTracker,           //its logical volume
                    "Tracker_PV",            //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // place the decay volume inside
  //                                   
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, -decayVolumeLength/2 ),
                    logicEnv,                //its logical volume
                    "Envelope_PV",              //its name
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
