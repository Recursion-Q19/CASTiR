
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "StepMax.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

// Local physic directly implemented in the Hadronthrapy directory
//#include "LocalIonIonInelasticPhysic.hh"             // Physic dedicated to the ion-ion inelastic processes
//#include "LocalINCLIonIonInelasticPhysic.hh"         // Physic dedicated to the ion-ion inelastic processes using INCL/ABLA

// Physic lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4DecayPhysics.hh"

//#include "OpticalPhysics.hh"
#include "G4OpticalPhysics.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
//#include "G4HadronQElasticPhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4Decay.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"

#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"

#include "G4RadioactiveDecayPhysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//=========================================================
PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 0.1*mm;
  GammaCut     = defaultCutValue;
  EminusCut  = defaultCutValue;
  EplusCut  = defaultCutValue;

  fHadr_Elast_Scat_Registered  = false;
  fBin_Cascade_Inleast_Scat_Registed  = false;
  fBin_Ion_Cascade_Inelast_Scat_Registed = false;
  fRadioactive_Decay_Registed = false;

  stepMaxProcess  = 0;

  pMessenger = new PhysicsListMessenger(this);

  SetVerboseLevel(1);

  // EM physics
  EMPhysicsList = new G4EmStandardPhysics_option3(1);
  G4cout<< ">> PhysicsList:: G4EmStandardPhysics_option3 activated by default << "<<G4endl;

  //Optical Physics
  OpticalPhysicsList = 0; // new OpticalPhysics(1, "optical_physics");

  // Deacy physics and all particles
  DecayPhysicsList = new G4DecayPhysics();
  G4cout<< ">> PhysicsList:: Decay activated by default<< "<<G4endl;
  G4cout<< ">> All other physics must be turned on manually<< "<<G4endl;

}

//=========================================================
PhysicsList::~PhysicsList()
{
  if(pMessenger!=0)      {delete pMessenger; pMessenger=0;}
  if(EMPhysicsList!=0)   {delete EMPhysicsList; EMPhysicsList =0;}
  if(OpticalPhysicsList) {delete OpticalPhysicsList; OpticalPhysicsList=0;}
  if(DecayPhysicsList)   {delete DecayPhysicsList; DecayPhysicsList=0;}
  for(size_t i=0; i<HadronPhysicsList.size(); i++) 
    {delete HadronPhysicsList[i]; HadronPhysicsList[i]=0;}
}

//=========================================================
void PhysicsList::ConstructParticle()
{
  DecayPhysicsList->ConstructParticle();
}

//=========================================================
void PhysicsList::ConstructProcess()
{
  // transportation
  AddTransportation();

  // electromagnetic physics list
  EMPhysicsList->ConstructProcess();
  em_config.AddModels();

  //optical physics list 
  if(OpticalPhysicsList!=0)OpticalPhysicsList->ConstructProcess();

  // decay physics list
  DecayPhysicsList->ConstructProcess();

  // hadronic physics lists
  for(size_t i=0; i<HadronPhysicsList.size(); i++) {
    HadronPhysicsList[i]->ConstructProcess();
  }

  // step limitation (as a full process)
  AddStepMax();
}

//=========================================================
void PhysicsList::AddPhysicsList(const G4String& name)
{

  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  //=========================================================
  //   ELECTROMAGNETIC MODELS
  //=========================================================

  if (name == "standard_opt3") {
    delete EMPhysicsList;
    EMPhysicsList = new G4EmStandardPhysics_option3();
    G4cout << "The following EM physics list has been activated: G4EmStandardPhysics_option3" << G4endl;}
  
  else if (name == "standard_opt4") {
    delete EMPhysicsList;
    EMPhysicsList = new G4EmStandardPhysics_option4();
    G4cout << "The following EM physics list has been activated: G4EmStandardPhysics_option4" << G4endl;} 
  
  else if (name == "LowE_Livermore") {
    delete EMPhysicsList;
    EMPhysicsList = new G4EmLivermorePhysics();
    G4cout << "The following EM physics list has been activated: G4EmLivermorePhysics" << G4endl;} 
  
  else if (name == "LowE_Penelope") {
    delete EMPhysicsList;
    EMPhysicsList = new G4EmPenelopePhysics();
    G4cout << "The following EM physics list has been activated: G4EmLivermorePhysics" << G4endl;}
  
  else if (name == "elastic" && !fHadr_Elast_Scat_Registered) {
    G4cout << "The following hadronic elastic physics list has been activated: G4HadronElasticPhysics" << G4endl;
    HadronPhysicsList.push_back( new G4HadronElasticPhysics());
    fHadr_Elast_Scat_Registered = true;} 
  
  // else if (name == "DElastic" && !fHadr_Elast_Scat_Registered) {
  //   HadronPhysicsList.push_back( new G4HadronDElasticPhysics());
  //   fHadr_Elast_Scat_Registered = true;} 

  // else if (name == "HElastic" && !fHadr_Elast_Scat_Registered) {
  //   HadronPhysicsList.push_back( new G4HadronHElasticPhysics());
  //   fHadr_Elast_Scat_Registered = true;} 

  // else if (name == "QElastic" && !fHadr_Elast_Scat_Registered) {
  //   HadronPhysicsList.push_back( new G4HadronQElasticPhysics());
  //   fHadr_Elast_Scat_Registered = true;} 

  else if (name == "binary" && !fBin_Cascade_Inleast_Scat_Registed) {
    HadronPhysicsList.push_back(new G4HadronInelasticQBBC());
    fBin_Cascade_Inleast_Scat_Registed = true;
    G4cout << "The following hadronic inelastic physics list has been activated: G4HadronInelasticQBBC" << G4endl;} 
  
  else if (name == "binary_ion" && !fBin_Cascade_Inleast_Scat_Registed) {
    HadronPhysicsList.push_back(new G4IonBinaryCascadePhysics());
    fBin_Cascade_Inleast_Scat_Registed = true;
    G4cout << "The following hadronic inelastic physics list has been activated: G4IonBinaryCascadePhysics" << G4endl;} 

  else if (name == "radioactive_decay" && ! fRadioactive_Decay_Registed ) {
    HadronPhysicsList.push_back(new G4RadioactiveDecayPhysics());
    fRadioactive_Decay_Registed = true;}

  else if (name == "optical_physics" ){
    if(OpticalPhysicsList!=0) delete OpticalPhysicsList; 
    OpticalPhysicsList = new G4OpticalPhysics(0, "optical_physics");
    G4cout << "The optical physics list:<< G4OpticalPhysics >> has been activated" << G4endl;} 
 
  else {G4cout << "PhysicsList::AddPhysicsList: <<" << name << ">>"<< " is not defined"<< G4endl;}
}

//=========================================================
void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  // stepMaxProcess = new StepMax();

  // theParticleIterator->reset();
  // while ((*theParticleIterator)()){
  //   G4ParticleDefinition* particle = theParticleIterator->value();
  //   G4ProcessManager* pmanager = particle->GetProcessManager();

  //   if (stepMaxProcess->IsApplicable(*particle) && pmanager)
  //     {
  // 	pmanager ->AddDiscreteProcess(stepMaxProcess);
  //     }
  // }

    // Step limitation seen as a process
  // StepMax*
    stepMaxProcess = new StepMax();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()){
      G4ParticleDefinition* particle = particleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (stepMaxProcess->IsApplicable(*particle))
        {
          pmanager ->AddDiscreteProcess(stepMaxProcess);
        }
  }  // Step limitation seen as a process
}

//=========================================================
void PhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(GammaCut, "gamma");
  SetCutValue(EminusCut, "e-");
  SetCutValue(EplusCut, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}

//=========================================================
void PhysicsList::SetCutForGamma(G4double cut)
{
  GammaCut = cut;
  SetParticleCuts(GammaCut, G4Gamma::Gamma());
}

//=========================================================
void PhysicsList::SetCutForElectron(G4double cut)
{
  EminusCut = cut;
  SetParticleCuts(EminusCut, G4Electron::Electron());
}

//=========================================================
void PhysicsList::SetCutForPositron(G4double cut)
{
  EplusCut = cut;
  SetParticleCuts(EplusCut, G4Positron::Positron());
}
