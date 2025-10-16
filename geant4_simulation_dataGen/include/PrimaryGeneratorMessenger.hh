

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
//--------------------------------------------------------------------

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* prim_gen);
   ~PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand* command, G4String newValue);
    
  private:
    PrimaryGeneratorAction*    primGenAction;
    
    G4UIdirectory*             primGenDir;

    G4UIcmdWithADoubleAndUnit*  setBeamEnergyCMD;
    G4UIcmdWithABool*           setFixedEnergyFlagCMD; 
    G4UIcmdWithAString*         initializeEnergySpectrumCMD;
};

//--------------------------------------------------------------------
#endif

