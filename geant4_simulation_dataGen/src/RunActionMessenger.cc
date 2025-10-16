

#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UImanager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

#include "globals.hh"

//------------------------------------------------------------------------------

RunActionMessenger::RunActionMessenger(RunAction* run_act)
:runAction(run_act)
{ 
  runActDir = new G4UIdirectory("/userRun/");
  runActDir->SetGuidance("UI commands specific to the User RunAction.\n");

  setOutputFileNameCMD =  new G4UIcmdWithAString("/userRun/setOutFileName", this);
  setOutputFileNameCMD->SetGuidance("set output file name\n");
  setOutputFileNameCMD->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//------------------------------------------------------------------------------

RunActionMessenger::~RunActionMessenger()
{
  delete setOutputFileNameCMD; 
  delete runActDir;
}

//------------------------------------------------------------------------------

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  // get the pointer to the User Interface manager
  //G4UImanager* UI = G4UImanager::GetUIpointer();
  if ( command == setOutputFileNameCMD)
    {
      runAction->SetOutputFileName(newValue);
    }
}
//------------------------------------------------------------------------------
