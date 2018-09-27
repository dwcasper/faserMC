// adapted from Geant4 example

#ifndef FaserDetectorConstruction_h
#define FaserDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4Cache.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"


class FaserGeometryMessenger;
class FaserTrackerGeometry;
class FaserFieldSetup;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4NistManager;
class G4Material;
class G4Region;

/// Detector construction class to define materials and geometry.

class FaserDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
  FaserDetectorConstruction();
  virtual ~FaserDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
    
  virtual void ConstructDecayVolume();
  virtual void ConstructTracker();
  virtual void ConstructCalorimeter();
  virtual void ConstructCalorimeterPlane();
  virtual void ConstructTrackerOld();
  virtual void ConstructTrackerPlane();
  virtual void ConstructTrackerModule();
  virtual void ConstructTrackerSensor();

  virtual void setReadoutStrips(G4int value) { sensor_readoutStrips = value; }
  virtual G4int getReadoutStrips() const { return sensor_readoutStrips; }

  virtual void setStripPitch(G4double value) { sensor_stripPitch = value; }
  virtual G4double getStripPitch() const { return sensor_stripPitch; }

  virtual void setStripLength(G4double value) { sensor_stripLength = value; }
  virtual G4double getStripLength() const { return sensor_stripLength; }

  virtual void setSensorGap(G4double value) { sensor_gap = value; }
  virtual G4double getSensorGap() const { return sensor_gap; }

  virtual void setSensorSizeZ(G4double value) { sensor_sizeZ = value; }
  virtual G4double getSensorSizeZ() const { return sensor_sizeZ; }

  virtual void setSensorStereoAngle(G4double value) { sensor_stereoAngle = value; }
  virtual G4double getSensorStereoAngle() const { return sensor_stereoAngle; }

  virtual void setSupportSizeZ(G4double value) { support_sizeZ = value; }
  virtual G4double getSupportSizeZ() const { return support_sizeZ; }

  virtual void setSensorPlanes(G4int value) { detector_sensorPlanes = value; }
  virtual G4int getSensorPlanes() const { return detector_sensorPlanes; }

  virtual void setCaloPlanes(G4int value) { detector_caloPlanes = value; }
  virtual G4int getCaloPlanes() const { return detector_caloPlanes; }

  virtual void setAbsorberX0(G4double value) { detector_absorberX0 = value; }
  virtual G4double getAbsorberX0() const { return detector_absorberX0; }

  virtual void setSamplerSizeZ(G4double value) { detector_samplerSizeZ = value; }
  virtual G4double getSamplerSizeZ() const { return detector_samplerSizeZ; }

  virtual void setPlanePitch(G4double value) { detector_planePitch = value; }
  virtual G4double getPlanePitch() const { return detector_planePitch; }

  virtual void setDecayVolumeLength(G4double value) { detector_decayVolumeLength = value; }
  virtual G4double getDecayVolumeLength() const { return detector_decayVolumeLength; }

  virtual void setTrackerLength(G4double value) { detector_trackerLength = value; }
  virtual G4double getTrackerLength() const { return detector_trackerLength; }

  virtual void setCalorimeterLength(G4double value) { detector_calorimeterLength = value; }
  virtual G4double getCalorimeterLength() const { return detector_calorimeterLength; }

  FaserTrackerGeometry * TrackerGeo() { return fTrackerGeo; }

  static constexpr G4double default_sensor_activeSizeY = 96.669*mm;
  static constexpr G4int    default_sensor_readoutStrips = 1280;
  static constexpr G4double default_sensor_stripPitch = 75.5*um;
  static constexpr G4double default_sensor_stripLength = 48.20*mm;
  static constexpr G4double default_sensor_gap = default_sensor_activeSizeY - 2 * default_sensor_stripLength;
  static constexpr G4double default_sensor_sizeZ = 0.32*mm;
  static constexpr G4double default_sensor_stereoAngle = 26.0*mrad;
  static constexpr G4double default_support_sizeZ = 3.3*mm;
  static constexpr G4int    default_detector_sensorPlanes = 8;
  static constexpr G4int    default_detector_caloPlanes = 6;
  static constexpr G4double default_detector_absorberX0 = 4.0;
  static constexpr G4double default_detector_samplerSizeZ = default_sensor_sizeZ;
  static constexpr G4double default_detector_planePitch = 5.0*cm;
  static constexpr G4double default_detector_decayVolumeLength = 3.0*m;
  static constexpr G4double default_detector_trackerLength = 2.0*m;
  static constexpr G4double default_detector_calorimeterLength = 0.25*m;

  virtual const G4LogicalVolume* GetTrackerStrip() const { return fLogicTrackerStrip; }

  protected:

  FaserGeometryMessenger* fGeometryMessenger;
  FaserTrackerGeometry* fTrackerGeo;
  G4LogicalVolume* fLogicCalorimeter;
  G4LogicalVolume* fLogicSampler;
  G4LogicalVolume* fLogicAbsorber;
  G4LogicalVolume* fLogicDecayVolume;
  G4LogicalVolume* fLogicTracker;
  G4LogicalVolume* fLogicTrackerPlane;
  G4LogicalVolume* fLogicCalorimeterPlane;
  G4LogicalVolume* fLogicTrackerModule;
  G4LogicalVolume* fLogicTrackerSensor;
  G4LogicalVolume* fLogicTrackerRow;
  G4LogicalVolume* fLogicTrackerStrip;

  // tunable (from macro) parameters 
  G4int    sensor_readoutStrips;
  G4double sensor_stripPitch;
  G4double sensor_stripLength;
  G4double sensor_gap;
  G4double sensor_sizeZ;
  G4double sensor_stereoAngle;
  G4double support_sizeZ;
  G4int    detector_sensorPlanes;
  G4int    detector_caloPlanes;
  G4double detector_absorberX0;
  G4double detector_samplerSizeZ;
  G4double detector_planePitch;
  G4double detector_decayVolumeLength;
  G4double detector_trackerLength;
  G4double detector_calorimeterLength;

  // these are not copied by the volumes that use them,
  // so they must not be changed

  G4RotationMatrix* fStereoPlus;
  G4RotationMatrix* fStereoMinus;
  G4RotationMatrix* fOverlapAngle;

  // mag field
  G4Cache<FaserFieldSetup*> fFieldSetup;

  // internal variables
  G4bool checkOverlaps;
  G4NistManager* nist;
  G4Region* fRegTracker;
  G4Region* fRegAir;
  G4Region* fRegCalo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

