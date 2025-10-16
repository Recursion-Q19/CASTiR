
 
#include "EventAction.hh"
#include "DataManager.hh"
#include "MCEvent.hh"


#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ios.hh"

//-------------------------------------------------------------------------------
 
EventAction::EventAction():dataManager(0)
{
  ev = new MCEvent;
  dataManager = DataManager::GetInstance();
  //dataManager->SetEvent(ev);
}

//-------------------------------------------------------------------------------
 
EventAction::~EventAction()
{
  if(ev) delete ev; 
}

//------------------------------------------------------------------------------
 
void EventAction::BeginOfEventAction(const G4Event* )
{

}

//------------------------------------------------------------------------------
 
void EventAction::EndOfEventAction(const G4Event* evnt)
{
  //dataManager->FillEvent();
  G4int event_id = evnt->GetEventID();
  
  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evnt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  //
  if (event_id < 5 || event_id%10000 == 0) { //if (event_id < 5 || event_id%100 == 0) {
    G4cout << ">>> Event " << evnt->GetEventID() << G4endl;
    G4cout << "    " << n_trajectories 
     	   << " trajectories stored in this event." << G4endl;
  }
}

//-----------------------------------------------------------------------------
