#ifndef DataManager_h
#define DataManager_h 1

#include "globals.hh"
#include "MCEvent.hh"
#include <fstream>

class TTree;
class TFile;

struct EventData {
    	unsigned ringA : 8;
	unsigned ringB : 8;
	unsigned detA : 16;
	unsigned detB : 16;
	unsigned layerA : 4;
	unsigned layerB : 4;
	unsigned reserved : 6;
	unsigned isRandom : 1;
	unsigned type : 1;
};

class DataManager //a singleton class
{
	public:
		static DataManager* GetInstance();

	private:
		DataManager():fFile(NULL), fFileName(0), fEventTree(NULL), fEvent(NULL) {}
		DataManager(DataManager const& );             //not defined, not copyable
		DataManager& operator= (DataManager const& ); //not defined, not assignable
		~DataManager();

	private:
		TFile*       fFile; 
		TString      fFileName;
		TString      fFileName1;
		TTree*       fEventTree;
		// static Int_t CerenTrackID;
		// static Int_t ScintTrackID;
		static Int_t TrackID;
		static Int_t new_event;
		static Int_t invalid_event;

		MCEvent*     fEvent;

		std::fstream fSTIRfile;
		std::fstream fCASTORfile;

	public:
		void open(Int_t RN);
		void open(const char* name);

		void close();
		void SetFileName(TString name) { fFileName=name; }

		void CleanUpEvent() { 
			if(fEvent) fEvent->Clear("C"); 
			// CerenTrackID =-1;
			// ScintTrackID =-1;
			TrackID =-1;
			new_event = 1;
			invalid_event = 0;
		}

		// void SetCerenkovTrackID(const Int_t &id) {CerenTrackID = id;}
		// void SetScintTrackID(const Int_t &id)    {ScintTrackID = id;}
		void SetTrackID(const Int_t &id)  {TrackID = id;}
		void SetNewEventFlag(const Int_t &flag) {new_event = flag;}
		bool GetNewEventFlag() {return new_event;}

		void SetInvalidEventFlag(const Int_t &flag) {invalid_event = flag;}
		bool GetInvalidEventFlag() {return invalid_event;}
	  
		void SetEventName(TString name) { fEvent->SetEventName(name); }
		void SetEventNumber(Int_t numb) { fEvent->SetEventNumber(numb);}

		//  void AddPrimary(EventPrimary* primary); 
		//  void AddOptTransmitted(opticalHit *hitPtr);   

		void SetEvent(MCEvent* ev) { fEvent = ev; }

		Int_t GetTrackID()   {return TrackID;}
		void FillEvent(){}; //SM: Added {}
		//  protected:
		MCEvent* GetEvent() { return fEvent; }
		void WriteSTIRdata(unsigned int crystalID1, unsigned int moduleID1, unsigned int crystalID2, unsigned int moduleID2);
		void WriteCASTORdata(unsigned int crystalID1, unsigned int moduleID1, unsigned int crystalID2, unsigned int moduleID2, unsigned long int time);
};

namespace
{ struct ForceSingletonInitialization
  { ForceSingletonInitialization() { DataManager::GetInstance(); } } GetInstance;
}

#endif


