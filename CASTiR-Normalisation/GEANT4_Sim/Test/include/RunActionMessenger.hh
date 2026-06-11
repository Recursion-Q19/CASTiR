

#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
//--------------------------------------------------------------------

class RunActionMessenger: public G4UImessenger
{
  public:
    RunActionMessenger(RunAction* run_act);
   ~RunActionMessenger();
    
    void SetNewValue(G4UIcommand* command, G4String newValue);
    
  private:
    RunAction*                 runAction;
    
    G4UIdirectory*             runActDir;
 
    G4UIcmdWithAString*        setOutputFileNameCMD;
};

//--------------------------------------------------------------------
#endif

