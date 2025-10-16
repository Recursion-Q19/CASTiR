
#include "RunAction.hh"
#include "RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"

#include "DataManager.hh"
#include "TBenchmark.h"

//------------------------------------------------------------------------------

RunAction::RunAction()
{
  dataManager = DataManager::GetInstance();
  bench = new TBenchmark();
  messenger = new RunActionMessenger(this);
  outputFilname = "";
}

//-----------------------------------------------------------------------------

RunAction::~RunAction()
{
  delete bench;	
  delete messenger;
}

//-----------------------------------------------------------------------------

void RunAction::BeginOfRunAction(const G4Run* )
{
  // save Rndm status
  //  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  // CLHEP::HepRandom::showEngineStatus();
  if(outputFilname=="")
    {
      std::ifstream runin;
      std::ofstream runout;
      G4int run_number = 0;
      // if the file "flux.runno" exists - use its contents as the lucid run number
      runin.open(".run_number.txt");
      if(runin.good())
	{
	  runin>>run_number;
	  runin.close();
	}
      
      //  aRun->SetRunID(run_number);
      // G4int runID = aRun->GetRunID();
      dataManager->open(run_number);
      run_number++;
      
      runout.open(".run_number.txt", std::ios::trunc);
      if(runout.good())
	{
	  runout << run_number << std::endl;
	  runout.close();
	}
      else
	{
	  G4cout<<__FILE__<<" : "<<__LINE__<<" :  .run_number.dat" << G4endl;
	}
    }
  else
    dataManager->open(outputFilname);

 
  bench->Reset();
  bench->Start("run_benchmark");
}
//----------------------------------------------------------------------------
void RunAction::SetOutputFileName(G4String name)
{
  outputFilname = name;
}
//-----------------------------------------------------------------------------

void RunAction::EndOfRunAction(const G4Run* )
{ 
  // G4int nEvnt = aRun->GetNumberOfEvent();
  dataManager->close();

  //show Rndm status
  //  CLHEP::HepRandom::showEngineStatus();
  bench->Show("run_benchmark");
}

//-----------------------------------------------------------------------------



