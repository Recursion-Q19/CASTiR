#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>

std::string inname = "";

void Usage(void);
void ParseCommandLineArguments(int narg, char* argv[]);

struct eventData {
    unsigned ringA : 8;
    unsigned ringB : 8;
    unsigned detA  : 16;
    unsigned detB  : 16;
    unsigned layerA: 4;
    unsigned layerB: 4;
    unsigned reserved: 6;
    unsigned isRandom: 1;
    unsigned type    : 1;
};

int main(int narg, char* argv[])
{
    ParseCommandLineArguments(narg, argv);
 
    std::fstream inFile(inname, std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error opening file " << inname << std::endl;
        return -1;
    }
   
    // We assume the first 32 bytes are a header, so skip that.
    inFile.seekg(32, std::ios::beg);
    std::streampos startPosition = inFile.tellg();

    // Move to the end of the file to compute total file size after header.
    inFile.seekg(0, std::ios::end);
    std::streampos endPosition = inFile.tellg();

    // Compute number of bytes of actual event data.
    std::streampos fileSize = endPosition - startPosition;
    // Divide by the size of our event structure to find how many events.
    auto numberOfEvents = fileSize / sizeof(eventData);

    std::cout << "Number of events in file: " << numberOfEvents << std::endl;

    inFile.close();
    return 0;
}


//----------------------------
// ParseCommandLineArguments
//----------------------------
void ParseCommandLineArguments(int narg, char* argv[])
{
  if(narg < 2) Usage();

  for(int i = 1; i < narg; i++)
  {
    std::string arg = argv[i];
    if(arg == "-h" || arg == "--help")
    {
      Usage();
    }
    else if(arg=="-inF")
    {
      if (i == narg - 1) {
        std::cout<<"-inF requires one argument!"<<std::endl;  
        Usage();
      }
      inname = argv[i+1];
    }
  }

  if(inname == "")
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
  std::cout<<"      exec_name -inF CASTOR_data.cdf"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"  options:"<<std::endl;
  std::cout<<" -h                      print this help message"<<std::endl;
  std::cout<<" -inF CASTOR_data.cdf    set the name of MC data file (required)"<<std::endl;
  exit(-2);
}
