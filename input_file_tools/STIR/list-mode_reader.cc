#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>

std::string inname = "";

void Usage(void);
void ParseCommandLineArguments(int narg, char* argv[]);

int ReadLMdata(std::fstream *fs);

struct eventData{	
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

int main(int narg, char* argv[])
{
  ParseCommandLineArguments(narg, argv);
  
  unsigned long int event_number =0;
  std::fstream inFile;
  inFile.open(inname.data(), std::ios::in | std::ios::binary);

  //int counter=0;
  
  if(inFile.is_open())
    {
	inFile.seekg(32, std::ios::beg);

	while(inFile.good())
		{
			ReadLMdata(&inFile);
			
			event_number++;
		} 
    }
	inFile.close();
	std::cout << "The number of Event: " << event_number << std::endl;
	return 0;
}


int ReadLMdata(std::fstream* fs)
{
	eventData data;
	fs->read(reinterpret_cast<char*>(&data), sizeof(data));

//Uncomment the following line if you want to print out the content of the binary input file  
/*
       if (fs) { // if (fs->good()) {
		    std::cout << "RingA: " << static_cast<int>(data.ringA) << 
		    "; RingB: " << static_cast<int>(data.ringB) << 
		    "; DetA: " << static_cast<int>(data.detA) << 
		    "; DetB: " << static_cast<int>(data.detB) << 
		    "; LayerA: " << static_cast<int>(data.layerA) <<
		    "; LayerB: " << static_cast<int>(data.layerB) << std::endl;
    	} else {
        	std::cerr << "Error reading from file" << std::endl;
    	}
*/
  return 0;
}



//----------------------------
// ParseCommandLineArguments
//----------------------------
void ParseCommandLineArguments(int narg, char* argv[])
{
  if(narg<1)Usage();

  for(int i=1; i<narg; i++)
    {
      std::string arg = argv[i];
      if(arg=="-h" || arg=="--help")
        {
          Usage();
        }
      else if(arg=="-inF")
        {
          if(i==narg-1)
            { std::cout<<"-inF requires one argument!"<<std::endl;  Usage(); }

          inname = argv[i+1];
        }
    }

  if(inname=="")
    {
      Usage();
    }

  std::cout<<" Input file name = "<<inname<<std::endl;
}


//----------------------------
// Usage
//----------------------------
void Usage(void)
{
  std::cout<<std::endl;
  std::cout<<"Usage:"<<std::endl;
  std::cout<<"      exec_name: -inF stir_inData.clm.safir"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"  options:"<<std::endl;
  std::cout<<" -h                      print this help message"<<std::endl;
  std::cout<<" -inF stir_inData.clm.safir set the name of MC data file (required)"<<std::endl;
  exit(-2);
}
