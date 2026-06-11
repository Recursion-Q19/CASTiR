
 
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

class G4Event;
class DataManager;
class MCEvent;

//-----------------------------------------------------------------------------

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
   ~EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event* evnt);
    virtual void EndOfEventAction(const G4Event* evnt);
  private:
    DataManager *dataManager;
    MCEvent     *ev;
};

//-----------------------------------------------------------------------------

#endif

    
