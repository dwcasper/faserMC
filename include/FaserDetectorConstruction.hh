// adapted from Geant4 example

#ifndef FaserDetectorConstruction_h
#define FaserDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4Cache.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserSensorPlaneConstruction;
class FaserGeometryMessenger;
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
  virtual void ConstructSampler();
  virtual void ConstructAbsorberPlane();
  virtual void ConstructCalorimeterModule();
  virtual void ConstructCalorimeter();

  virtual G4NistManager* getNistManager() const { return nist; }

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

  virtual void setSensorPlanes(G4int value) { tracker_sensorPlanes = value; }
  virtual G4int getSensorPlanes() const { return tracker_sensorPlanes; }

  virtual void setSamplerPlanes(G4int value) { sampler_sensorPlanes = value; }
  virtual G4int getSamplerPlanes() const { return sampler_sensorPlanes; }

  virtual void setAbsorberX0(G4double value) { sampler_absorberX0 = value; }
  virtual G4double getAbsorberX0() const { return sampler_absorberX0; }

  virtual void setCaloPlanes(G4int value) { calo_planes = value; }
  virtual G4int getCaloPlanes() const { return calo_planes; }

  virtual void setCaloTowers(G4int value) { calo_towers = value; }
  virtual G4int getCaloTowers() const { return calo_towers; }

  virtual void setCaloModules(G4int value) { calo_modules = value; }
  virtual G4int getCaloModules() const { return calo_modules; }

  virtual void setCaloScintThickness(G4double value) { calo_scintThickness = value; }
  virtual G4double getCaloScintThickness() const { return calo_scintThickness; }

  virtual void setCaloAbsorbThickness(G4double value) { calo_absorbThickness = value; }
  virtual G4double getCaloAbsorbThickness() const { return calo_absorbThickness; }

  virtual void setCaloPlaneXY(G4double value) { calo_planeXY = value; }
  virtual G4double getCaloPlaneXY() const { return calo_planeXY; }

  virtual void setCaloModuleXY(G4double value) { calo_moduleXY = value; }
  virtual G4double getCaloModuleXY() const { return calo_moduleXY; }

  virtual void setSamplerSizeZ(G4double value) { detector_samplerLength = value; }
  virtual G4double getSamplerSizeZ() const { return detector_samplerLength; }

  virtual void setPlanePitch(G4double value) { detector_planePitch = value; }
  virtual G4double getPlanePitch() const { return detector_planePitch; }

  virtual void setDecayVolumeLength(G4double value) { detector_decayVolumeLength = value; }
  virtual G4double getDecayVolumeLength() const { return detector_decayVolumeLength; }

  virtual void setTrackerLength(G4double value) { detector_trackerLength = value; }
  virtual G4double getTrackerLength() const { return detector_trackerLength; }

  virtual void setCalorimeterLength(G4double value) { detector_calorimeterLength = value; }
  virtual G4double getCalorimeterLength() const { return detector_calorimeterLength; }

  static constexpr G4double default_sensor_activeSizeY = 96.669*mm;
  static constexpr G4int    default_sensor_readoutStrips = 1280;
  static constexpr G4double default_sensor_stripPitch = 75.5*um;
  static constexpr G4double default_sensor_stripLength = 48.20*mm;
  static constexpr G4double default_sensor_gap = default_sensor_activeSizeY - 2 * default_sensor_stripLength;
  static constexpr G4double default_sensor_sizeZ = 0.32*mm;
  static constexpr G4double default_sensor_stereoAngle = 26.0*mrad;
  static constexpr G4double default_support_sizeZ = 3.3*mm;
  static constexpr G4int    default_tracker_sensorPlanes = 8;
  static constexpr G4int    default_sampler_sensorPlanes = 6;
  static constexpr G4double default_sampler_absorberX0 = 4.0;
  static constexpr G4int    default_calo_planes = 67;
  static constexpr G4int    default_calo_towers = 1;
  static constexpr G4int    default_calo_modules = 4;
  static constexpr G4double default_calo_scintThickness = 4.0*mm;
  static constexpr G4double default_calo_absorbThickness = 2.0*mm;
  static constexpr G4double default_calo_tyvekThickness = 120.0*um;
  static constexpr G4double default_calo_planeXY = 12.12*cm;
  static constexpr G4double default_calo_moduleXY = 12.14*cm;
  static constexpr G4double default_detector_samplerLength = default_sensor_sizeZ;
  static constexpr G4double default_detector_planePitch = 5.0*cm;
  static constexpr G4double default_detector_decayVolumeLength = 3.0*m;
  static constexpr G4double default_detector_trackerLength = 2.0*m;
  static constexpr G4double default_detector_calorimeterLength = 0.25*m;

  virtual const G4LogicalVolume* GetTrackerStrip() const;

  protected:

  FaserGeometryMessenger* fGeometryMessenger;
  G4LogicalVolume* fLogicCalorimeter;
  G4LogicalVolume* fLogicSampler;
  G4LogicalVolume* fLogicAbsorber;
  G4LogicalVolume* fLogicDecayVolume;
  G4LogicalVolume* fLogicTracker;
  G4LogicalVolume* fLogicTrackerPlane;
  G4LogicalVolume* fLogicSamplerPlane;
  G4LogicalVolume* fLogicCaloModule;

  // tunable (from macro) parameters 
  G4int    sensor_readoutStrips;
  G4double sensor_stripPitch;
  G4double sensor_stripLength;
  G4double sensor_gap;
  G4double sensor_sizeZ;
  G4double sensor_stereoAngle;
  G4double support_sizeZ;
  G4int    tracker_sensorPlanes;
  G4int    sampler_sensorPlanes;
  G4double sampler_absorberX0;
  G4int    calo_planes;
  G4int    calo_towers;
  G4int    calo_modules;
  G4double calo_scintThickness;
  G4double calo_absorbThickness;
  G4double calo_tyvekThickness;
  G4double calo_planeXY;
  G4double calo_moduleXY;
  G4double detector_samplerLength;
  G4double detector_planePitch;
  G4double detector_decayVolumeLength;
  G4double detector_trackerLength;
  G4double detector_calorimeterLength;

  // these are not copied by the volumes that use them,
  // so they must not be changed
  G4RotationMatrix* fSamplerRotation;

  // mag field
  G4Cache<FaserFieldSetup*> fFieldSetup;

  // internal variables
  G4bool checkOverlaps;
  G4NistManager* nist;
  G4Region* fRegTracker;
  G4Region* fRegCalorimeter;

  FaserSensorPlaneConstruction* fTrackerFactory;
  FaserSensorPlaneConstruction* fSamplerFactory;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

