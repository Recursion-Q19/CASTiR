

#include "SteppingAction.hh"
#include "G4SteppingManager.hh"

#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4GeometryTolerance.hh"
#include "G4UnitsTable.hh"

#include "DataManager.hh"
#include "MCEvent.hh"

#include "G4Gamma.hh"
#include "G4TransportationManager.hh"

//------------------------------------------------------------------------------

SteppingAction::SteppingAction()
{ 
  dataManager = DataManager::GetInstance();
  aTrack = 0;
  preStepPoint  = 0;
  preStepPV     = 0;
  preStepPVname = "";
  hitPos  = G4ThreeVector(0.*mm, 0.*mm, 0.*mm);
  vertPos = G4ThreeVector(0.*mm, 0.*mm, 0.*mm);

  proc_name = "";
  particle_name = "";

  postStepPoint = 0;
  postStepPV    = 0;
  preStepPVname ="";

  trackID=0;

  cID1 = 0;
  mID1 = 1000;
  name1="";
  
  cID2 = 0;
  mID2 = 1000;
  name2="";
  //optTransm = new opticalHit();
}

//-------------------------------------------------------------------------------
SteppingAction::~SteppingAction()
{
  //if(optTransm)   {delete optTransm; optTransm=0;}
}
//-------------------------------------------------------------------------------

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	aTrack 		= aStep->GetTrack();
	preStepPoint 	= aStep->GetPreStepPoint();
	preStepPV 	= preStepPoint->GetTouchableHandle()->GetVolume();  
	preStepPVname 	= preStepPV->GetName();


	if(aTrack->GetDefinition() == G4Gamma::GammaDefinition())
	{ 
		postStepPoint 	= aStep->GetPostStepPoint();
		postStepPV 	= postStepPoint->GetTouchableHandle()->GetVolume();

		if(postStepPV)
		{
			postStepPVname 	= postStepPV->GetName();
			trackID 	= aTrack->GetTrackID();

			 //G4cout<<postStepPVname<<G4endl;
			if(postStepPVname.contains("pShield") || dataManager->GetInvalidEventFlag() == 1)
			{ 
				aTrack->SetTrackStatus(fStopAndKill);
				if(dataManager->GetInvalidEventFlag() == 0){ 
					dataManager->SetInvalidEventFlag(1);
					//G4cout << "Photon XXXXX in pShield" << G4endl;
				}else{
					dataManager->SetInvalidEventFlag(0);
					//G4cout << "Photon YYYYY might be in pShield" << G4endl;
				}
			}else if(postStepPVname.contains("pvCrystal"))
			{

				if(dataManager->GetNewEventFlag()==1) //1 means it is a new event
				{
					cID1 = postStepPoint->GetTouchableHandle()->GetCopyNumber(0);
					mID1 = postStepPoint->GetTouchableHandle()->GetCopyNumber(1);
					dataManager->SetNewEventFlag(0); //0 means old event
//G4cout << "Photon AAAAA" << G4endl;
					aTrack->SetTrackStatus(fStopAndKill);
					name1=postStepPVname;
				}else{
					cID2 = postStepPoint->GetTouchableHandle()->GetCopyNumber(0);
					mID2 = postStepPoint->GetTouchableHandle()->GetCopyNumber(1);
//G4cout << "Photon BBBBB" << G4endl;
					aTrack->SetTrackStatus(fStopAndKill);
					name2=postStepPVname;
				}
		  
				unsigned long int time = 25;//aTrack->GetGlobalTime()*1000000000;

				if(mID1!=mID2 && dataManager->GetNewEventFlag()==0 && mID1!=1000 && mID2 !=1000)
				{
					dataManager->SetNewEventFlag(true);
					dataManager->WriteSTIRdata(cID1, mID1, cID2, mID2);
					dataManager->WriteCASTORdata(cID1, mID1, cID2, mID2, time);
//G4cout << "Writing WWWWWWWW" << G4endl;
					cID1 = 0; mID1 = 1000; cID2=0; mID2=1000;
				}
				aTrack->SetTrackStatus(fStopAndKill);
			}
		}
      	}
}

