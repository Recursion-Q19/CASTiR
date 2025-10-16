
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class DataManager;
class crystalHit;
class opticalHit;
class G4Track;
class G4StepPoint;
class G4VPhysicalVolume;


//-----------------------------------------------------------------------------

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  ~SteppingAction();

  void UserSteppingAction(const G4Step*);
  
private:
  DataManager* dataManager;
  G4Track*     aTrack;

  G4StepPoint* preStepPoint;
  G4VPhysicalVolume* preStepPV;
  G4String preStepPVname;

  G4ThreeVector hitPos;
  G4ThreeVector vertPos;

  G4String proc_name;
  G4String particle_name;

  G4StepPoint* postStepPoint;
  G4VPhysicalVolume* postStepPV;
  G4String postStepPVname;
  G4int trackID;

  unsigned int cID1;
  unsigned int mID1;
  G4String name1;


  unsigned int cID2;
  unsigned int mID2;
  G4String name2;

  opticalHit   *optTransm;
};

//-----------------------------------------------------------------------------

#endif
