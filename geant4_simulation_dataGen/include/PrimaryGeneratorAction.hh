
 
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "G4Navigator.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"

#include <vector>

class G4ParticleGun;
class G4Event;
class G4ParticleTable;
class G4ParticleDefinition;
class PrimaryGeneratorMessenger;
class DataManager;
class EventPrimary;

//-------------------------------------------------------------------------------
 
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();    
  ~PrimaryGeneratorAction();
  
public:
  virtual void GeneratePrimaries(G4Event* anEvent);
  
  void SetParticleEnergy(G4double e)   {particleEnergy = e;}
  void SetFixedEnergyFlag(G4bool flag) {fixedEnergyFlag = flag;}
private:
  void SampleEnergy(G4double probability); 
  void SampleDirection();

public:  
  void InitializeSpectrum(G4String name);

  G4double GetSpectrumIntegral() {return integral;}
	G4ThreeVector rPosition(std::vector<G4String> volumeNames);

private:
  G4ParticleTable       *particleTable;
  G4ParticleDefinition  *particle;
  G4ParticleGun         *particleGun;

  DataManager           *dataManager;
  EventPrimary          *eventPrimary;
	G4LogicalVolumeStore* 	logicalVolumeStore;
	G4Navigator* 		navigator;

  // particle direction sampler
  G4ThreeVector          particleDirection;  
  G4bool                 parallelBeamFlag;


  // particle energy sampler
  G4double               particleEnergy;
  G4bool                 fixedEnergyFlag;
  G4double               integral;    //used by generator internally

  std::vector<G4double> energyV;       //used by generator internally
  std::vector<G4double> intensityV;    //used by generator internally
  std::vector<G4double> intensityVCI;  //used by generator internally
  std::pair<std::vector<G4double>::iterator, std::vector<G4double>::iterator> position;

  // Messenger for communication with UI:
  PrimaryGeneratorMessenger* messenger;
};


//--------------------------------------------------------------------------------

#endif


