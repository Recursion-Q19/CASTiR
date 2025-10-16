//We run this using GEANT4v10.7

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


#include "globals.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"
//---------------------------------------------------------------------------------

int main(int argc,char **argv)
{
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4int seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  // User Verbose output class
  //
  //  G4VSteppingVerbose* verbosity = new SteppingVerbose();
  // G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  G4RunManager * runManager = new G4RunManager();

  // User Initialization classes (mandatory)
  DetectorConstruction* detector = new DetectorConstruction(1.0*m);
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);   
  //
  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);

  //============================================ 
  // Initialize Data manager class             =
  //============================================
  //DataManager *dataManager = DataManager::GetInstance();

  //============================================ 
  // User Action classes                       =
  //============================================
  G4UserRunAction* run_action = new RunAction();
  runManager->SetUserAction(run_action);
  
  G4UserEventAction* event_action = new EventAction();
  runManager->SetUserAction(event_action);

  G4UserSteppingAction* stepping_action =  new SteppingAction();
  runManager->SetUserAction(stepping_action);  
 
  //  Initialize G4 kernel
  // runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager * UI = G4UImanager::GetUIpointer();  

  if (ui==0)   // batch mode  
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }
  else           // interactive mode : define visualization and UI terminal
    { 
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();

      UI->ApplyCommand("/control/execute run.mac");
      
      ui->SessionStart();
      //      UI->ApplyCommand("/control/execute macro/vis.mac");  
      delete ui;

      delete visManager;
    }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  //  delete dataManager;
  // delete materials;
  delete runManager;
  // delete verbosity;

  return 0;
}

//-----------------------------------------------------------------------------
