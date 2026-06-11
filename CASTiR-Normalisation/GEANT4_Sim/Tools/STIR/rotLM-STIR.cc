#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>

std::string inname = "";
std::string outname = "";
int rot = 0;

void Usage(void);
void ParseCommandLineArguments(int narg, char* argv[]);

int LMdataTransformerSTIR(std::fstream* fsRead, std::fstream* fsWrite, int rot);

struct EventData{	
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

//%%%%%%%%%%%%%main function%%%%%%%%%%%%%%%%%%%%%%%%%

int main(int narg, char* argv[])
{
  ParseCommandLineArguments(narg, argv);
  
  unsigned long int event_number =0;
  
  std::fstream inFile;
  inFile.open(inname.data(), std::ios::in | std::ios::binary);
  
  std::fstream outFile;
  outFile.open(outname.data(), std::ios::out | std::ios::binary);
  
  const char *signature = "SAFIR CListModeData\0";
  outFile.write(signature, strlen(signature) + 1);
  outFile.seekp(12, std::ios::cur);

  //int counter=0;
  
  if(inFile.is_open() && outFile.is_open())
    {
	inFile.seekg(32, std::ios::beg);

	while(LMdataTransformerSTIR(&inFile, &outFile, rot))
		{			
			event_number++;
		} 
    }
    
	inFile.close();
	outFile.close();
	
	std::cout << "The number of Event: " << event_number << std::endl;
	return 0;
}

//%%%%%%%%%%%%%%%%%%Implementation%%%%%%%%%%%%%%%%%%%%%%%%%%%

int LMdataTransformerSTIR(std::fstream* fsRead, std::fstream* fsWrite, int rot)//Rotates
{
	EventData eventData;
	fsRead->read(reinterpret_cast<char*>(&eventData), sizeof(eventData));

//Uncomment the following line if you want to print out the content of the binary input file  
/*
       if (fs) { // if (fs->good()) {
		    std::cout << "RingA: " << static_cast<int>(eventData.ringA) << 
		    "; RingB: " << static_cast<int>(eventData.ringB) << 
		    "; DetA: " << static_cast<int>(eventData.detA) << 
		    "; DetB: " << static_cast<int>(eventData.detB) << 
		    "; LayerA: " << static_cast<int>(eventData.layerA) <<
		    "; LayerB: " << static_cast<int>(eventData.layerB) << std::endl;
    	} else {
        	std::cerr << "Error reading from file" << std::endl;
    	}
    	
*/

//std::cout << "rotation: " << (rot*45) << " degrees" << std::endl; 
	
	if(fsRead->good()){
	
		if(((static_cast<int>(eventData.detA)) + 35*rot ) < 280){
	   		eventData.detA += 35*rot; 
	   	} else {
	   		eventData.detA -= (280 - 35*rot);
	   	}
		if(((static_cast<int>(eventData.detB)) + 35*rot) < 280) {
			eventData.detB += 35*rot;
		} else {
			eventData.detB -= (280 - 35*rot);
		}

	   	fsWrite->write(reinterpret_cast<const char*>(&eventData), sizeof(eventData));
	   	
	   	return 1;
   	}
   	
  return 0;
}



//----------------------------
// ParseCommandLineArguments
//----------------------------
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
				if (arg=="-rot")
					{
						rot = std::stoi(argv[i+1]);
					}
				else
					{
						Usage();
					}
				break;
			case 3:
				if (arg=="-inF")
					{
						inname = argv[i+1];
					}
				else
					{
						Usage();
					}
				break;
			case 5:
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
  std::cout<<" rotation = " << rot <<std::endl;
}


//----------------------------
// Usage
//----------------------------
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
