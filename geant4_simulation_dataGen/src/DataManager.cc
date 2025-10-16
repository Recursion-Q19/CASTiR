
#include "DataManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"


DataManager* DataManager::GetInstance()
{ static DataManager *pointerToTheSingletonInstance = new DataManager();
  return pointerToTheSingletonInstance;
}

// Int_t DataManager::CerenTrackID = -1;
// Int_t DataManager::ScintTrackID = -1;
Int_t DataManager::TrackID = -1;
Int_t DataManager::new_event = 1;
Int_t DataManager::invalid_event = 0;
//------------------------------------------------------------------------

DataManager::~DataManager()
{
  if(fEvent)     delete fEvent;
  if(fEventTree) delete fEventTree;
  if(fFile)      delete fFile;
}

//-----------------------------------------------------------------------

void DataManager::open(G4int RN)
{

 
  fFileName = "mc_data_";
  std::ostringstream tmp_str;
  tmp_str << RN;
 fFileName+= tmp_str.str();
  fFileName+= ".root";
  
  fFile = new TFile(fFileName, "RECREATE");

  TString tree_comment="tree, Run # ";
  tree_comment+=tmp_str.str();

  fEventTree = new TTree("event_tree", tree_comment);
  fEventTree->SetDirectory(fFile);
  fEventTree->SetAutoSave(5000000); // autosave when every 5Mb
  fEventTree->SetCacheSize(10000000);  //set a 10 MBytes cache (useless when writing local files)
  fEventTree->Branch("EventBranch", "MCEvent", &fEvent, 512000, 99);

	tmp_str.str("");
	tmp_str.clear();
	//tmp_str << RN;
  
  fFileName = "mc_data_";
  tmp_str << RN;
  fFileName+= tmp_str.str();
  fFileName+= ".cdf";
  fCASTORfile.open(fFileName, std::ios::out | std::ios::binary); 

	tmp_str.str("");
	tmp_str.clear();
	//tmp_str << RN;

  fFileName = "mc_data_";
  tmp_str << RN;
  fFileName+= tmp_str.str();
  fFileName+= ".clm.safir";
  fSTIRfile.open(fFileName, std::ios::out | std::ios::binary);
  const char *signature = "SAFIR CListModeData\0";
  fSTIRfile.write(signature, strlen(signature) + 1);
  fSTIRfile.seekp(12, std::ios::cur);
}

//-----------------------------------------------------------------------

void DataManager::open(const char* name)
{
  
  fFileName = name;
  fFileName1 = name;
  
 fFile = new TFile(fFileName, "RECREATE");

  TString tree_comment="tree, Run # ";
  tree_comment+= fFileName;

  fEventTree = new TTree("event_tree", tree_comment);
  fEventTree->SetDirectory(fFile);
  fEventTree->SetAutoSave(5000000); // autosave when every 5Mb
  fEventTree->SetCacheSize(10000000);  //set a 10 MBytes cache (useless when writing local files)
  fEventTree->Branch("EventBranch", "MCEvent", &fEvent, 512000, 99); 
  //--------------------------//

  int n = fFileName.Sizeof();
  fFileName.Resize(n-6);
  fFileName+= ".cdf";
  fCASTORfile.open(fFileName, std::ios::out | std::ios::binary);

  int m = fFileName1.Sizeof();
  fFileName1.Resize(m-6);
  fFileName1+= ".clm.safir";
  fSTIRfile.open(fFileName1, std::ios::out | std::ios::binary);
  const char *signature = "SAFIR CListModeData\0";
  fSTIRfile.write(signature, strlen(signature) + 1);
  fSTIRfile.seekp(12, std::ios::cur);
}


//---------------------------------------------------
void DataManager::close()
{

  fSTIRfile.close();
  fCASTORfile.close();

  if(fFile)
    {
      fFile->Write(); 
      fFile->Close();
    }
}

//--------------------------------------------------------//SM Just this kine

void DataManager::WriteSTIRdata(unsigned int crystalID1, unsigned int moduleID1, unsigned int crystalID2, unsigned int moduleID2)
{  
	//const char *signature = "SAFIR CListModeData\0";
	//fCASTORfile.write(signature, strlen(signature) + 1);
	//fCASTORfile.seekp(12, std::ios::cur);
    	
	unsigned int numCrystals1D = 35;
	unsigned int numDetectorsPerRing = 8;
   

	unsigned int i1 = crystalID1%numCrystals1D;
	unsigned int j1 = crystalID1/numCrystals1D;
	unsigned int t1 = moduleID1%numDetectorsPerRing;
	//unsigned int kkk1 = moduleID1/numDetectorsPerRing;

	unsigned int DetA = i1 + t1*numCrystals1D; 
	unsigned int ModA = j1;// + kkk1*numCrystals1D; 

   	unsigned int i2 = crystalID2%numCrystals1D;
   	unsigned int j2 = crystalID2/numCrystals1D;
	unsigned int t2 = moduleID2%numDetectorsPerRing;
	//unsigned int kkk2 = moduleID2/numDetectorsPerRing;
   
	unsigned int DetB = i2 + t2*numCrystals1D;
	unsigned int ModB = j2;// + kkk2*numCrystals1D; 

	EventData eventData;
	eventData.ringA = ModA;       
    	eventData.ringB = ModB;    	
	eventData.detA = DetA;      
    	eventData.detB = DetB;
	eventData.layerA = 0;
	eventData.layerB = 0;
	eventData.reserved = 0;      
    	eventData.isRandom = 0;    
    	eventData.type = 0;

   	fSTIRfile.write(reinterpret_cast<const char*>(&eventData), sizeof(eventData));
    	
}
//------------------------------------------------

void DataManager::WriteCASTORdata(unsigned int crystalID1, unsigned int moduleID1, unsigned int crystalID2, unsigned int moduleID2, unsigned long int time)
{ 
	double temp = time/1000000.;
	temp *=4;
	unsigned int t = temp;
	fCASTORfile.write((char *)&t, sizeof(unsigned int));
	// float a = 1;    //attenuation correction for this event
	// fCASTORfile.write((char *)&a, sizeof(float));
	// float s = 0;   //un-normalized scatter rate (counts/s)
	// fCASTORfile.write((char *)&s, sizeof(float));
	// float r = 0;    //un-normalized randoms' rate (counts/s)
	// fCASTORfile.write((char *)&r, sizeof(float));
	// float n = 1;    //nomralization factor for this event
	// fCASTORfile.write((char *)&n, sizeof(float));
	// short k=1;      //number of crystal pairs
	// fCASTORfile.write((char *)&k, sizeof(short));

	unsigned int numCrystals1D = 35;
	unsigned int numDetectorsPerRing = 8;
	// unsigned int numRings = 1;
	   

	unsigned int i = crystalID1%numCrystals1D;
	unsigned int j = crystalID1/numCrystals1D;
	unsigned int ttt = moduleID1%numDetectorsPerRing;
		//unsigned int kkk;
	//if (moduleID1/numDetectorsPerRing == 1)
	//{
	//	kkk = 8;
	//}else {kkk =0;}

	unsigned int castorID1 = i+j*numDetectorsPerRing*numCrystals1D + ttt*numCrystals1D;// + kkk*numCrystals1D*numCrystals1D; 

	i = crystalID2%numCrystals1D;
	j = crystalID2/numCrystals1D;
	ttt = moduleID2%numDetectorsPerRing;
		//unsigned int kkk2;
	//if (moduleID2/numDetectorsPerRing == 1)
	//{
	//	kkk2 = 8;
	//}else {kkk2 =0;}
	   
	unsigned int castorID2 = i+j*numDetectorsPerRing*numCrystals1D + ttt*numCrystals1D;// + kkk2*numCrystals1D*numCrystals1D;

	//unsigned int numCrystalsPerModule = numCrystals1D*numCrystals1D;   
	// unsigned int castorID1 = crystalID1 + numCrystalsPerModule*moduleID1;
	// unsigned int castorID2 = crystalID2 + numCrystalsPerModule*moduleID2;
	   
	fCASTORfile.write((char *)&castorID1, sizeof(unsigned int));
	fCASTORfile.write((char *)&castorID2, sizeof(unsigned int));
}

//------------------------------------------------
