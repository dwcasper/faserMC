#include "FaserSensorPlaneConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"

FaserSensorPlaneConstruction::FaserSensorPlaneConstruction(FaserDetectorConstruction* parent, G4String name)
    : fParent(parent), fName(name), fLogicStrip(nullptr), fLogicSensor(nullptr), fLogicRow(nullptr),fLogicModule(nullptr)
    , fStereoPlus(nullptr), fStereoMinus(nullptr), fOverlapAngle(nullptr), checkOverlaps(true)
{

}

FaserSensorPlaneConstruction::~FaserSensorPlaneConstruction()
{
    if (fStereoMinus) delete fStereoMinus;
    if (fStereoPlus) delete fStereoPlus;
    if (fOverlapAngle) delete fOverlapAngle;
}

void FaserSensorPlaneConstruction::ConstructSensor()
{
  // sensor dimensions
  //
  // In x, there are field-shaping strips at each end, and we assume a border on each edge of sensor_gap/2
  //
  G4double sensor_sizeX = ( fParent->getReadoutStrips() + 2 ) * fParent->getStripPitch() + fParent->getSensorGap();

  // In y, there are two rows of strips, separated by a gap, and we assume border on each edge of sensor_gap/2
  //
  G4double sensor_sizeY = ( fParent->getStripLength() * 2) + (fParent->getSensorGap() * 2);
  G4double sensor_sizeZ = fParent->getSensorSizeZ();
  
  G4cout << "Sensor dimensions for " << fName << " : ( " << sensor_sizeX/mm << " mm, " << sensor_sizeY/mm << " mm, "
	 << sensor_sizeZ/mm << " mm )" << G4endl; 

  // sensor volume
  //
  G4NistManager* nist = fParent->getNistManager();
  G4Material* sensor_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box* solidSensor =    
    new G4Box(G4String(fName + "Sensor"),                       //its name
       0.5*sensor_sizeX, 0.5*sensor_sizeY, 0.5*sensor_sizeZ);     //its size
      
  fLogicSensor =                         
    new G4LogicalVolume(solidSensor,          //its solid
                        sensor_mat,           //its material
                        G4String(fName + "Sensor") );            //its name
  
  // strip row - this contains only the active volume of the strips
  //
  G4Box* solidRow = 
    new G4Box(G4String(fName + "Row"),
	      0.5*(fParent->getReadoutStrips() * fParent->getStripPitch()), 0.5*fParent->getStripLength(), 0.5*sensor_sizeZ);
  fLogicRow =
    new G4LogicalVolume(solidRow,
			sensor_mat,
			G4String(fName + "Row"));

  // place the two rows inside the sensor
  new G4PVPlacement(0,
		    G4ThreeVector(0, -0.5*(fParent->getStripLength() + fParent->getSensorGap()), 0),
		    fLogicRow,
		    G4String(fName + "Row_PV"),
		    fLogicSensor,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(0,
		    G4ThreeVector(0, +0.5*(fParent->getStripLength() + fParent->getSensorGap()), 0),
		    fLogicRow,
		    G4String(fName + "Row_PV"),
		    fLogicSensor,
		    false,
		    1,
		    checkOverlaps);
  
  // a single strip; this will be replicated
  //	      
  G4Box* solidStrip =
    new G4Box(G4String(fName + "Strip"),
	      0.5 * fParent->getStripPitch(), 0.5 * fParent->getStripLength(), 0.5 * sensor_sizeZ);
  fLogicStrip = 
    new G4LogicalVolume(solidStrip,
		       sensor_mat,
		       G4String(fName + "Strip"));

  // place the requested number of strips inside the row
  //
  new G4PVReplica(G4String(fName + "Strip_PV"),
		  fLogicStrip,
		  fLogicRow,
		  kXAxis,
		  fParent->getReadoutStrips(),
		  fParent->getStripPitch(),
		  0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserSensorPlaneConstruction::ConstructModule()
{
  // Construct a single Si sensor region
  //
  ConstructSensor();

  const G4Box* sSensor = dynamic_cast<const G4Box*>(fLogicSensor->GetSolid());
  G4double sensor_sizeX = 2*sSensor->GetXHalfLength();
  G4double sensor_sizeY = 2*sSensor->GetYHalfLength();
  G4double sensor_sizeZ = 2*sSensor->GetZHalfLength();

  G4double sensor_stereoAngle = fParent->getSensorStereoAngle();

  // sensor stereo angle (+/-)
  //
  G4cout << fName << " stereo angle: " << sensor_stereoAngle/mrad << " mrad" << G4endl;

  // create rotation matrices for both senses of rotation
  // we have to hold these pointers (and not change the objects)
  // until the end of the job

  // this combination of rotations keeps the local +z axis 
  // of the strips pointing out of the plane 

  fStereoPlus = new G4RotationMatrix;
  fStereoPlus->rotateZ(sensor_stereoAngle);

  fStereoMinus = new G4RotationMatrix;
  fStereoMinus->rotateY(CLHEP::pi);
  fStereoMinus->rotateZ(sensor_stereoAngle);

  // support dimensions - assume a 2:1 rectangular shape
  //
  G4double support_sizeX = sensor_sizeX * cos(sensor_stereoAngle) + sensor_sizeY * sin(sensor_stereoAngle);
  G4double support_sizeY = sensor_sizeY * cos(sensor_stereoAngle) + sensor_sizeX * sin(sensor_stereoAngle) 
    + sensor_sizeY/cos(sensor_stereoAngle);
  G4double support_sizeZ = fParent->getSupportSizeZ();
  G4cout << fName << " support dimensions: " << support_sizeX/mm << " mm (X), "
	 << support_sizeY/mm << " mm (Y), " << support_sizeZ/mm << " mm (Z)" << G4endl;

  //support volume
  //
  G4NistManager* nist = fParent->getNistManager();

  G4Material* support_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");
  G4Box* solidSupport =
    new G4Box(fName + "Support", 0.5*support_sizeX, 0.5*support_sizeY, 0.5*support_sizeZ);

  G4LogicalVolume* logicSupport =
    new G4LogicalVolume(solidSupport,
			support_mat,
			G4String(fName + "Support"));

  // module dimensions
  //
  G4double module_sizeX = support_sizeX;
  G4double module_sizeY = support_sizeY;
  G4double module_sizeZ = 2 * sensor_sizeZ + support_sizeZ;

  // module volume
  //
  G4Material* module_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidModule = 
    new G4Box(G4String(fName + "Module"), 0.5*module_sizeX, 0.5*module_sizeY, 0.5*module_sizeZ);

  fLogicModule = 
    new G4LogicalVolume(solidModule,
			module_mat,
			G4String(fName + "Module"));

  // place support inside module volume
  //
  new G4PVPlacement(0,                     //no rotation
                    G4ThreeVector(),       //at (0,0,0)
                    logicSupport,          //its logical volume
                    G4String(fName + "Support_PV"),          //its name
                    fLogicModule,   //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
  
  // place sensors inside module volume
  // the same logical volume is re-used, but it is translated and rotated four different ways
  //
  new G4PVPlacement(fStereoPlus,
            G4ThreeVector(0.0, 0.5*sensor_sizeY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
	        fLogicSensor,
	        G4String(fName + "Sensor_PV"),
	        fLogicModule,
	        false,
	        0,
	        checkOverlaps);

  new G4PVPlacement(fStereoMinus,
		    G4ThreeVector(0.0, 0.5*sensor_sizeY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    fLogicSensor,
		    G4String(fName + "Sensor_PV"),
		    fLogicModule,
		    false,
		    1,
		    checkOverlaps);

  new G4PVPlacement(fStereoPlus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
		    fLogicSensor,
		    G4String(fName + "Sensor_PV"),
		    fLogicModule,
		    false,
		    2,
		    checkOverlaps);

  new G4PVPlacement(fStereoMinus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    fLogicSensor,
		    G4String(fName + "Sensor_PV"),
		    fLogicModule,
		    false,
		    3,
		    checkOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* FaserSensorPlaneConstruction::Construct()
{

  // Construct a tracker module (two of them overlap to form a plane)
  ConstructModule();

  const G4Box* sModule = dynamic_cast<const G4Box*>(fLogicModule->GetSolid());
  G4double module_sizeX = 2*sModule->GetXHalfLength();
  G4double module_sizeY = 2*sModule->GetYHalfLength();
  G4double module_sizeZ = 2*sModule->GetZHalfLength();

  // effective half-width of the wafer due to stereo rotation
  //
  G4double sensor_stereoAngle = fParent->getSensorStereoAngle();
  const G4Box* sSensor = dynamic_cast<const G4Box*>(fLogicSensor->GetSolid());
  G4double sensor_sizeX = 2*sSensor->GetXHalfLength();
  G4double wPrime = (sensor_sizeX/2) / cos(sensor_stereoAngle);

  // overlap angle that will allow maximum x-separation without any gap
  //
  G4double overlapAngle = asin( module_sizeZ / wPrime )/2;
  
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
  G4cout << fName << " plane dimensions: " << plane_sizeX/mm << " mm (X), "
	 << plane_sizeY/mm << " mm (Y), " << plane_sizeZ/mm << " mm (Z)" << G4endl;

  // plane volume
  //
  G4NistManager* nist = fParent->getNistManager();
  G4Material* plane_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidPlane =
    new G4Box(G4String(fName + "Plane"), 0.5*plane_sizeX, 0.5*plane_sizeY, 0.5*plane_sizeZ);

  G4LogicalVolume* logicSensorPlane =
    new G4LogicalVolume(solidPlane,
                        plane_mat,
                        G4String(fName + "Plane"));

  // place modules inside plane
  //
  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(-xOffset, 0, 0),
		    fLogicModule,
		    G4String(fName + "Module_PV"),
		    logicSensorPlane,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(+xOffset, 0, 0),
		    fLogicModule,
		    G4String(fName + "Module_PV"),
		    logicSensorPlane,
		    false,
		    1,
 		    checkOverlaps);
  return logicSensorPlane;
}
