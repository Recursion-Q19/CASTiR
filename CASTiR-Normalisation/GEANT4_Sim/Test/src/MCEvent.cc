
#include "MCEvent.hh"

ClassImp(opticalHit)
ClassImp(EventPrimary)
ClassImp(MCEvent)

//-------------------------------------------------------------------------------
opticalHit::opticalHit():TObject()
{
  trackID         = -1;
  parentID        = -1;
  local_time      = 0.;
  global_time     = 0.;
  vertexPos       = TVector3();
  hitPos          = TVector3();
  enenergy        = 0.;
  crystalID1       = -1;//SM
  crystalID2       = -1;//SM
  moduleID1        = -1;//SM
  moduleID2        = -1;//SM
}

//--------------------------------------------------------------------------------
opticalHit::opticalHit(const opticalHit & orig):TObject(orig)
{
  trackID         = orig.trackID;
  parentID        = orig.parentID;
  local_time      = orig.local_time;
  global_time     = orig.global_time;
  vertexPos       = orig.vertexPos;
  hitPos          = orig.hitPos;
  enenergy        = orig.enenergy;
  crystalID1       = orig.crystalID1;//SM
  crystalID2       = orig.crystalID2;//SM
  moduleID1        = orig.moduleID1;//SM
  moduleID2        = orig.moduleID2;//SM
}
//--------------------------------------------------------------------------------

void opticalHit::Clear(Option_t *)
{
  trackID         = -1;
  parentID        = -1;
  local_time      = 0.;
  global_time     = 0.;
  vertexPos       = TVector3();
  hitPos          = TVector3();
  enenergy        = 0.;
  crystalID1       = -1;//SM
  crystalID2       = -1;//SM
  moduleID1        = -1;//SM
  moduleID2        = -1;//SM
}

//________________________________________________________________________________
EventPrimary::EventPrimary(): TObject()
{
  fTrackID       = 0;
  fName          = ""; 
  fMass          = 0.;
  fCharge        = 0.;
  fTotalEnergy   = 0.; 
  fMomentum.SetXYZ(0.,0.,0.);
  fVertextPos.SetXYZ(0., 0., 0.);
  fCrystalID1       = 0;//SM
  fCrystalID2       = 0;//SM
  fModuleID1        = 0;//SM
  fModuleID2        = 0;//SM
}

//______________________________________________________________________________
EventPrimary::EventPrimary(const EventPrimary& orig):TObject(orig)
{
  fTrackID       = orig.fTrackID;
  fName          = orig.fName; 
  fMass          = orig.fMass;
  fCharge        = orig.fCharge;
  fTotalEnergy   = orig.fTotalEnergy; 
  fMomentum      = orig.fMomentum;
  fVertextPos    = orig.fVertextPos;
  fCrystalID1       = orig.fCrystalID1;//SM
  fCrystalID2       = orig.fCrystalID2;//SM
  fModuleID1        = orig.fModuleID1;//SM
  fModuleID2        = orig.fModuleID2;//SM
}

//________________________________________________________________________________
void EventPrimary::Clear(Option_t*)
{
  fTrackID       = 0; 
  fName          = "";
  fMass          = 0; 
  fCharge        = 0.;
  fTotalEnergy   = 0.;
  fMomentum.SetXYZ(0.,0.,0.);
  fVertextPos.SetXYZ(0., 0., 0.);
  fCrystalID1       = 0;//SM
  fCrystalID2       = 0;//SM
  fModuleID1        = 0;//SM
  fModuleID2        = 0;//SM
}

//______________________________________________________________________________
MCEvent::MCEvent() : TObject()
{
  fEventName        = "";
  fEventNumber      = 0;
  fN_Primaries      = 0;
  fN_OptTransmitted = 0;
  fEventPrimaries   = new TClonesArray("EventPrimary", 20);
  fOpticalHits      = new TClonesArray("opticalHit", 100);
}
//______________________________________________________________________________

MCEvent::MCEvent(const MCEvent& orig) : TObject(orig)
{
  fEventName          = orig.fEventName;
  fEventNumber        = orig.fEventNumber;
  fN_Primaries        = orig.fN_Primaries;
  fN_OptTransmitted   = orig.fN_OptTransmitted;
  fEventPrimaries     = orig.fEventPrimaries;
  fOpticalHits        = orig.fOpticalHits;
}

//______________________________________________________________________________
MCEvent::~MCEvent()
{
  Clear("C");
  if(fEventPrimaries)    {delete fEventPrimaries; fEventPrimaries=0;}  
  if(fOpticalHits)       {delete fOpticalHits; fOpticalHits=0;}
}

//______________________________________________________________________________
EventPrimary* MCEvent::AddPrimary(EventPrimary* primaryPtr)
{
   TClonesArray &evPrimaries = *fEventPrimaries;
   EventPrimary *primary = new(evPrimaries[fN_Primaries++]) (EventPrimary)(*primaryPtr);
  
   return primary;
}

//______________________________________________________________________________
opticalHit *MCEvent::AddOptTransmitted(opticalHit *hitPtr)
{
   TClonesArray &tgtHits = *fOpticalHits;
   opticalHit *hit = new(tgtHits[fN_OptTransmitted++]) (opticalHit)(*hitPtr);

   return hit;
}

//______________________________________________________________________________
void MCEvent::Clear(Option_t *opt)
{
  fEventName          = "";
  fEventNumber        = 0;
  fN_Primaries        = 0;
  fN_OptTransmitted   = 0;
  if(fEventPrimaries)fEventPrimaries->Clear(opt);
  if(fOpticalHits)fOpticalHits->Clear(opt);
}

