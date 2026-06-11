

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
//#include "G4String.hh"

//------------------------------------------------------------------------------
class G4Run;
class DataManager;
class TBenchmark;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  ~RunAction();

public:
  virtual void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  void SetOutputFileName(G4String name);
private:
  TBenchmark         *bench;
  DataManager        *dataManager; 
  RunActionMessenger *messenger;
  G4String            outputFilname;
};

//------------------------------------------------------------------------------

#endif





