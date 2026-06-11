
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
//int DataManager::numLines=0;
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
/*
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
  */
}

//--------------------------------------SM----------------------------------
/*
void DataManager::map_initializer(){
	for(int i=0; i<(35*70*8); i++){
		for(int j=i+1; j<(35*70*8); j++){
			histmode[{i,j}] = 0;
		}
	}
}
*/
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
/*
  int m = fFileName1.Sizeof();
  fFileName1.Resize(m-6);
  fFileName1+= ".clm.safir";
  fSTIRfile.open(fFileName1, std::ios::out | std::ios::binary);
  const char *signature = "SAFIR CListModeData\0";
  fSTIRfile.write(signature, strlen(signature) + 1);
  fSTIRfile.seekp(12, std::ios::cur);
*/
}


//---------------------------------------------------
void DataManager::close()
{

  fCASTORfile.close();

  if(fFile)
    {
      fFile->Write(); 
      fFile->Close();
    }
}

//--------------------------------------------------------//SM Just this kine

void DataManager::hist_map(unsigned int crystalID1, unsigned int moduleID1, unsigned int crystalID2, unsigned int moduleID2)
{  
	unsigned int numCrystals1D = 35;
	unsigned int numDetectorsPerRing = 8;

	unsigned int i = crystalID1%numCrystals1D;
	unsigned int j = crystalID1/numCrystals1D;
	unsigned int ttt = moduleID1%numDetectorsPerRing;
		
	unsigned int castorID11 = i+j*numDetectorsPerRing*numCrystals1D + ttt*numCrystals1D;

	i = crystalID2%numCrystals1D;
	j = crystalID2/numCrystals1D;
	ttt = moduleID2%numDetectorsPerRing;
	   
	unsigned int castorID22 = i+j*numDetectorsPerRing*numCrystals1D + ttt*numCrystals1D;	
	
	uint16_t castorID1 = static_cast<uint16_t>(castorID11);
	uint16_t castorID2 = static_cast<uint16_t>(castorID22);

	//histmode[{castorID1,castorID2}] = histmode[{castorID1,castorID2}] + 1.;
	histmode[{castorID1,castorID2}] = histmode[{castorID1,castorID2}] + 1;

	//numLines ++;
	//std::cout << "histmode: " << castorID1 << "=====" << histmode[{castorID1,castorID2}] << std::endl;
}

//------------------------------------------------

void DataManager::WriteCASToRdataHist(unsigned long int time)
{ 
	double temp = time/1000000.;
  	temp *=4;
  	unsigned int t = temp;
/*
	for(std::map<std::set<unsigned int>, float>::iterator it_map = histmode.begin(); it_map != histmode.end(); it_map++){

		fCASTORfile.write((char *)&t, sizeof(unsigned int));
		fCASTORfile.write((char *)&it_map->second, sizeof(float));

		for(std::set<unsigned int>::iterator it_set=(it_map->first).begin(); it_set != (it_map->first).end(); it_set++){
			fCASTORfile.write((char *)&(*it_set), sizeof(unsigned int));
		}
	}
*/

for(std::map<std::set<uint16_t>, uint16_t>::iterator it_map = histmode.begin(); it_map != histmode.end(); it_map++){

		fCASTORfile.write((char *)&t, sizeof(unsigned int));
		
		float counts_line = static_cast<float>(it_map->second);
		fCASTORfile.write((char *)&counts_line, sizeof(float));

		for(std::set<uint16_t>::iterator it_set=(it_map->first).begin(); it_set != (it_map->first).end(); it_set++){
			unsigned int crystalID = static_cast<unsigned int>(*it_set);
			fCASTORfile.write((char *)&crystalID, sizeof(unsigned int));
		}
	}


}

//------------------------------------------------
