
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class StepMax;
class PhysicsListMessenger;

class PhysicsList: public G4VModularPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  void ConstructParticle();

  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);

  void AddPhysicsList(const G4String& name);
  void ConstructProcess();

  void AddStepMax();
  StepMax* GetStepMaxProcess() {return stepMaxProcess;};

private:

  G4EmConfigurator em_config;

  G4double GammaCut;
  G4double EminusCut;
  G4double EplusCut;

  G4bool   fHadr_Elast_Scat_Registered;
  G4bool   fBin_Cascade_Inleast_Scat_Registed;
  G4bool   fBin_Ion_Cascade_Inelast_Scat_Registed;
  G4bool   fRadioactive_Decay_Registed;

  G4VPhysicsConstructor*               EMPhysicsList;
  G4VPhysicsConstructor*               DecayPhysicsList;
  std::vector<G4VPhysicsConstructor*>  HadronPhysicsList;

  G4VPhysicsConstructor*               OpticalPhysicsList;

  StepMax*                             stepMaxProcess;

  PhysicsListMessenger*                pMessenger;
};

#endif
