// adapted from Geant4 example

#include "FaserDetectorConstruction.hh"
#include "FaserGeometryMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserDetectorConstruction::FaserDetectorConstruction()
  : G4VUserDetectorConstruction(), fGeometryMessenger(new FaserGeometryMessenger(this)),
    sensor_sizeXY(default_sensor_sizeXY), 
    sensor_sizeZ(default_sensor_sizeZ),
    sensor_stereoAngle(default_sensor_stereoAngle),
    support_sizeZ(default_support_sizeZ),
    fStereoPlus(nullptr), fStereoMinus(nullptr), fOverlapAngle(nullptr)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserDetectorConstruction::~FaserDetectorConstruction()
{ 
  if (fGeometryMessenger) delete fGeometryMessenger;
  if (fStereoPlus) delete fStereoPlus;
  if (fStereoMinus) delete fStereoMinus;
  if (fOverlapAngle) delete fOverlapAngle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* FaserDetectorConstruction::Construct()
{  

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // Get nist material manager
  //
  G4NistManager* nist = G4NistManager::Instance();
  
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

  // sensor dimensions
  //
  G4cout << "Sensor dimensions: " << sensor_sizeXY/mm << " mm (XY), " << sensor_sizeZ/mm << " mm (Z)" << G4endl; 

  // sensor volume
  //
  G4Material* sensor_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box* solidSensor =    
    new G4Box("Sensor",                       //its name
       0.5*sensor_sizeXY, 0.5*sensor_sizeXY, 0.5*sensor_sizeZ);     //its size
      
  G4LogicalVolume* logicSensor =                         
    new G4LogicalVolume(solidSensor,          //its solid
                        sensor_mat,           //its material
                        "Sensor");            //its name

  // support dimensions - assume a 2:1 rectangular shape
  //
  G4double support_sizeX = sensor_sizeXY * (cos(sensor_stereoAngle) + sin(sensor_stereoAngle));
  G4double support_sizeY = support_sizeX + sensor_sizeXY/cos(sensor_stereoAngle);
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
  G4double module_sizeX = support_sizeX, module_sizeY = support_sizeY;
  G4double module_sizeZ = 2 * sensor_sizeZ + support_sizeZ;

  // module volume
  //
  G4Material* module_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidModule = 
    new G4Box("Module", 0.5*module_sizeX, 0.5*module_sizeY, 0.5*module_sizeZ);

  G4LogicalVolume* logicModule = 
    new G4LogicalVolume(solidModule,
			module_mat,
			"Module");

  // place support inside module volume
  //
  new G4PVPlacement(0,                     //no rotation
                    G4ThreeVector(),       //at (0,0,0)
                    logicSupport,          //its logical volume
                    "Support",             //its name
                    logicModule,           //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
  
  // place sensors inside module volume
  // the same logical volume is re-used, but it is translated and rotated four different ways
  //
  new G4PVPlacement(fStereoMinus,
                    G4ThreeVector(0.0, 0.5*sensor_sizeXY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
	            logicSensor,
	            "Sensor_00",
	            logicModule,
	            false,
	            0,
	            checkOverlaps);

  new G4PVPlacement(fStereoPlus,
		    G4ThreeVector(0.0, 0.5*sensor_sizeXY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    logicSensor,
		    "Sensor_01",
		    logicModule,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(fStereoMinus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeXY/cos(sensor_stereoAngle), support_sizeZ/2 + sensor_sizeZ/2),
		    logicSensor,
		    "Sensor_10",
		    logicModule,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(fStereoPlus,
		    G4ThreeVector(0.0, -0.5*sensor_sizeXY/cos(sensor_stereoAngle), -(support_sizeZ/2 + sensor_sizeZ/2)),
		    logicSensor,
		    "Sensor_11",
		    logicModule,
		    false,
		    0,
		    checkOverlaps);


  // effective half-width of the wafer due to stereo rotation
  //
  G4double wPrime = (sensor_sizeXY/2) / cos(sensor_stereoAngle);

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

  G4LogicalVolume* logicPlane =
    new G4LogicalVolume(solidPlane,
                        plane_mat,
                        "Plane");

  // place modules inside plane
  //
  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(-xOffset, 0, 0),
		    logicModule,
		    "Module_0",
		    logicPlane,
		    false,
		    0,
		    checkOverlaps);

  new G4PVPlacement(fOverlapAngle,
		    G4ThreeVector(+xOffset, 0, 0),
		    logicModule,
		    "Module_1",
		    logicPlane,
		    false,
		    0,
 		    checkOverlaps);


    /*
  // detector width
  //
  G4double detector_sizeXY = 2 * support_sizeXY;

  // number of modules (planes)
  //
  G4int module_count = 5;

  // Z distance between plane centers
  //
  G4double module_pitch = 20.0*cm;

  // Detector length (front surface of first plane to back surface of last)
  //
  G4double detector_sizeZ = (module_count - 1) * module_pitch + module_sizeZ;

  // decay volume (full) length
  //
  G4double decay_lengthZ = 10.0*m;

  // distance from veto inner surface to any part of detector and decay volume
  //
  G4double veto_gap = 25.0*cm;

  // veto thickness - all surfaces of air region
  //
  G4double veto_thickness = 15.0*cm; 

  // half dimensions of air volume
  //
  G4double airVolume_sizeXY = detector_sizeXY + 2 * vetoGap;
  G4double airVolume_sizeZ = decay_lengthZ + detector_sizeZ + 2 * veto_gap;

  // half (outer) dimensions of the veto
  //
  G4double vetoVolume_sizeXY = airVolume_sizeXY + 2 * veto_thickness;
  G4double vetoVolume_sizeZ = airVolume_sizeZ + 2 * veto_thickness;

  // and the world volume
  // this is sized so that the middle of the front-most sensor defines z = 0
  // regardless of how everything else is laid out
  //
  G4double forward_distance = sensor_sizeZ/2 + decay_lengthZ + veto_gap + veto_thickness;
  G4double backward_distance = detector_sizeZ - sensor_sizeZ/2 + veto_gap + veto_thickness;
  
  G4double world_sizeXY = vetoVolume_sizeXY + 2.0*m;
  G4double world_sizeZ = std::max(forward_distance, backward_distance) + 2.0*m;
  */

  G4double world_sizeXY = 8 * sensor_sizeXY, world_sizeZ = 50.0*cm;
  //     
  // World
  //
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_CONCRETE");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  /*
  //
  // Veto                     
  //
  G4Material* veto_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");

  G4Box* solidVeto =    
    new G4Box("Veto",                    //its name
        0.5*veto_sizeXY, 0.5*veto_sizeXY, 0.5*veto_sizeZ); //its size
      
  G4LogicalVolume* logicVeto =                         
    new G4LogicalVolume(solidVeto,            //its solid
                        veto_mat,             //its material
                        "Veto");             //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicVeto,               //its logical volume
                    "Veto",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  */
  //  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4double env_sizeXY = world_sizeXY - 10.0*cm, env_sizeZ = world_sizeZ - 10.0*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  new G4PVPlacement(0,
		    G4ThreeVector(),
		    logicPlane,
		    "Plane_0",
		    logicEnv,
		    false,
		    0,
		    checkOverlaps);

  /*
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //     
  // Shape 1
  //  
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos1 = G4ThreeVector(0, 2*cm, -7*cm);
        
  // Conical section shape       
  G4double shape1_rmina =  0.*cm, shape1_rmaxa = 2.*cm;
  G4double shape1_rminb =  0.*cm, shape1_rmaxb = 4.*cm;
  G4double shape1_hz = 3.*cm;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  G4Cons* solidShape1 =    
    new G4Cons("Shape1", 
    shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb, shape1_hz,
    shape1_phimin, shape1_phimax);
                      
  G4LogicalVolume* logicShape1 =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape1",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //     
  // Shape 2
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4ThreeVector pos2 = G4ThreeVector(0, -1*cm, 7*cm);

  // Trapezoid shape       
  G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
  G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
  G4double shape2_dz  = 6*cm;      
  G4Trd* solidShape2 =    
    new G4Trd("Shape2",                      //its name
              0.5*shape2_dxa, 0.5*shape2_dxb, 
              0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size
                
  G4LogicalVolume* logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
  //
  //always return the physical World
  //
  */
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
