#ifndef MCEvent_H
#define MCEvent_H
                                            
#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TVector3.h"

//------------------------------------------------------------
class opticalHit : public TObject {

private:  
  Int_t    trackID;
  Int_t    parentID;
  Double_t local_time;
  Double_t global_time;
  Double_t enenergy;
  TVector3 vertexPos;
  TVector3 hitPos;
  Int_t    crystalID1;
  Int_t    crystalID2;
  Int_t    moduleID1;
  Int_t    moduleID2;
 

public:
  opticalHit();
  opticalHit(const opticalHit& orig);
  virtual ~opticalHit() {}

  virtual void Clear(Option_t*);

  void SetTrackID(const Int_t &id)      {trackID = id;}
  void SetParentID(const Int_t &id)     {parentID = id;}
  void SetGlobalTime(const Double_t &t) {global_time = t;}
  void SetLocalTime(const Double_t &t)  {local_time = t;}
  void SetVertexPos(const TVector3 &v)  {vertexPos = v;}
  void SetHitPos(const TVector3 &v)     {hitPos = v;}
  void SetEnergy(const Double_t &e)     {enenergy = e;}
  void SetCrystalID1(const unsigned int &cID1)     {crystalID1 = cID1;} //SM
  void SetCrystalID2(const unsigned int &cID2)     {crystalID2 = cID2;} //SM
  void SetModuleID1(const unsigned int &mID1)     {moduleID1 = mID1;} //SM
  void SetModuleID2(const unsigned int &mID2)     {moduleID2 = mID2;} //SM

  Int_t    GetTrackID()    {return trackID;}
  Int_t    GetParentID()   {return parentID;}
  Double_t GetGlobalTime() {return global_time;}
  Double_t GetLocalTime()  {return local_time;}
  TVector3 GetVertexPos()  {return vertexPos;}
  TVector3 GetHitPos()     {return hitPos;}
  Double_t GetWaveLnegth() {return 1240./enenergy;}
  Int_t    GetCrystalID1() {return crystalID1;}//SM
  Int_t    GetCrystalID2() {return crystalID2;}//SM
  Int_t    GetModuleID1()  {return moduleID1;}//SM
  Int_t    GetModuleID2()  {return moduleID2;}//SM

  ClassDef(opticalHit,1)  //[Analyze] Crystal Event /*do not remove this comment*/
};


//------------------------------------------------------------------------------
class EventPrimary : public TObject 
{
public:
  EventPrimary();
  EventPrimary(const EventPrimary& orig);
  virtual ~EventPrimary() {};

  virtual void Clear(Option_t*);

  void SetTrackID(const Int_t &id)                { fTrackID       = id;       }
  void SetName(const TString &name)               { fName          = name;     }
  void SetMass(const Double_t &mass)              { fMass          = mass;     } 
  void SetCharge(const Double_t &charge)          { fCharge        = charge;   }
  void SetTotalEnergy(const Double_t &energy)     { fTotalEnergy   = energy;   }
  void SetMomentum(const TVector3 &momentum)      { fMomentum      = momentum; }
  void SetVertexPosition(const TVector3 &pos)     { fVertextPos    = pos;      } 
  void SetCrystalID1(const unsigned int &cID1)     { fCrystalID1    = cID1;      } //SM
  void SetCrystalID2(const unsigned int &cID2)     { fCrystalID2    = cID2;      } //SM
  void SetModuleID1(const unsigned int &mID1)     { fModuleID1    = mID1;      } //SM
  void SetModuleID2(const unsigned int &mID2)     { fModuleID2    = mID2;      } //SM

  Int_t    GetPrimaryTrackID() { return fTrackID;      }
  TString  GetPrimaryName()    { return fName;         }
  Double_t GetMass()           { return fMass;         }
  Double_t GetCharge()         { return fCharge;       }
  Double_t GetTotalEnergy()    { return fTotalEnergy;  }
  TVector3 GetMomentum()       { return fMomentum;     }
  TVector3 GetVertexPosition() { return fVertextPos;   }
  Int_t    GetCrystalID1()     { return fCrystalID1;   }//SM
  Int_t    GetCrystalID2()     { return fCrystalID2;   }//SM
  Int_t    GetModuleID1()      { return fModuleID1;    }//SM
  Int_t    GetModuleID2()      { return fModuleID2;    }//SM

private:
  Int_t    fTrackID; 
  TString  fName;
  Double_t fMass;
  Double_t fCharge;
  Double_t fTotalEnergy;
  TVector3 fVertextPos;
  TVector3 fMomentum;
  Int_t    fCrystalID1;
  Int_t    fCrystalID2;
  Int_t    fModuleID1;
  Int_t    fModuleID2;

  ClassDef(EventPrimary,1)  //[Analyze] event primary /*do not remove this comment*/
};


class MCEvent : public TObject {

private:  
  TString        fEventName;       //name in character format
  Int_t          fEventNumber;  

  Int_t          fN_Primaries;
  Int_t          fN_OptTransmitted;

  TClonesArray*  fEventPrimaries;    //->array with primaries
  TClonesArray*  fOpticalHits;      //->array of transmitted Cerenkov photons/*do not remove this comment*/

public:
  MCEvent();
  MCEvent(const MCEvent& orig);
  virtual ~MCEvent();

  void Clear(Option_t *option ="");
  
  void SetEventNumber(Int_t e_num )     { fEventNumber = e_num; }
  void SetEventName(TString name)       { fEventName   = name; }  

  EventPrimary *AddPrimary(EventPrimary *primaryPtr);
  opticalHit   *AddOptTransmitted(opticalHit *hitPtr);

  TString       GetEventName()      {return fEventName;}
  Int_t         GetEventNumber()    {return fEventNumber;}
  Int_t         GetN_Primaries()    {return fN_Primaries;}
  Int_t         GetN_OptTransm()    {return fN_OptTransmitted;}

  TClonesArray *GetPrimaries()       { return fEventPrimaries; }
  TClonesArray *GetOptTransmitted()  { return fOpticalHits;}

  ClassDef(MCEvent,1)  //[Analyze] MCEvent structure /*do not remove this comment*/
};


#endif
