#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <map>
#include <set>

using namespace std;

//===============================================
// function prototypes
//===============================================

std::string inname = "";
std::string outname = "";

void Usage(void);

void ParseCommandLineArguments(int narg, char* argv[]);

int transferCASToR_HistData(fstream* fsRead, fstream* fsWrite);

//void writeTransformedHist(unsigned long int time, fstream *fs);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//===============================================
// main function
//===============================================

int main(int narg, char* argv[]){

	//map<set<unsigned int>, float> histData;
	ParseCommandLineArguments(narg, argv);
	
	cout << "Double Check - Input: " << argv[2] << endl;
	cout << "Double Check - Output: " << argv[4] << endl;	
	
	fstream inFile;
	inFile.open(inname.data(), ios::in | ios::binary);
	
	if(!inFile.is_open()){
		cerr << "Failed to open: " << inname << endl;
	}
	
	fstream outFile;
	outFile.open(outname, ios::out | ios::binary); 
	
	if(!outFile.is_open()){
		cerr << "Failed to open: " << outname << endl;
	}
	
	while(inFile.good()){
		transferCASToR_HistData(&inFile, &outFile);	
	}
	
	inFile.close();
	
	
	
	//writeTransformedHist(1000000, &outFile);	
	
	outFile.close();
	
	

	return 0;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//===============================================
// reading data from binary file
//===============================================
int transferCASToR_HistData(fstream* fsRead, fstream* fsWrite){

	unsigned int t;
	float numCoincidences;
	unsigned int crystalID1;
	unsigned int crystalID2;
	
	
	if((fsRead->read((char *)&t, sizeof(unsigned int))) && (fsRead->read((char *)&numCoincidences, sizeof(float))) && 
		(fsRead->read((char *)&crystalID1, sizeof(unsigned int))) && (fsRead->read((char *)&crystalID2, sizeof(unsigned int)))){

		//histData[{crystalID1, crystalID2}] = histData[{crystalID1, crystalID2}] + numCoincidences;
		
		fsWrite->write((char *)&t, sizeof(unsigned int));
			
		fsWrite->write((char *)&numCoincidences, sizeof(float));
		
		if((crystalID1 % 280) < 245){crystalID1 += 35;}else{crystalID1 -= 245;}
		if((crystalID2 % 280) < 245){crystalID2 += 35;}else{crystalID2 -= 245;}
		fsWrite->write((char *)&crystalID1, sizeof(unsigned int));
		fsWrite->write((char *)&crystalID2, sizeof(unsigned int));
	}
	
	return 0;
}

//===============================================
// parseTerminalArguments
//===============================================

void ParseCommandLineArguments(int narg, char* argv[])
{
  if(narg<1)Usage();

  for(int i=1; i < narg; i+=2)
    {
      std::string arg = argv[i];
      if(arg=="-h" || arg=="--help")
        {
          Usage();
        }
      else
        {
          switch (i)
			{
			case 1:
				if (arg=="-inF")
					{
						inname = argv[i+1];
					}
				else
					{
						Usage();
					}
				break;
			case 3:
				if (arg=="-outF")
					{
						outname = argv[i+1];
					}
				else
					{
						Usage();
					}
				break;
			default:
				Usage();
				break;
			}
        }
    }

  if(inname=="")
    {
      Usage();
    }

  std::cout<<" Input file name = " << inname <<std::endl;
  std::cout<<" output file name = " << outname <<std::endl;
}


//===============================================
// Usage
//===============================================
void Usage(void)
{
  std::cout<<std::endl;
  std::cout<<"Usage:"<<std::endl;
  std::cout<<"      exec_name:  -rot rotation_degrees -inF stir_inData.clm.safir -outF rotated_output.clm.safir" <<std::endl;
  std::cout<<std::endl;
  std::cout<<"  options:"<<std::endl;
  std::cout<<" -h                      print this help message"<<std::endl;
  std::cout<<" -rot rotation_degrees set the rotation in degrees (required)"<<std::endl;
  std::cout<<" -inF stir_inData.clm.safir set the name of MC input data file (required)"<<std::endl;
  std::cout<<" -outF rotated_output.clm.safir set the name of the rotated output file (required)"<<std::endl;
  exit(-2);
}
