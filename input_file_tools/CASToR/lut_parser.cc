#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>

std::string inname = "";

void Usage(void);
void ParseCommandLineArguments(int narg, char* argv[]);

int ReadLUTdata(std::fstream *fs);

int main(int narg, char* argv[])
{
  ParseCommandLineArguments(narg, argv);
  
  
  std::fstream inFile;
  inFile.open(inname, std::ios::in | std::ios::binary);

  int counter=0;
  
  if(inFile.is_open())
    {
      while(inFile.good())
	{
	  std::cout<<counter;
	  ReadLUTdata(&inFile);
	  ++counter;
	} 
    }
    
  inFile.close();
  return 0;
}


int ReadLUTdata(std::fstream *fs)
{

  float x=0, y=0, z=0, nx=0, ny=0, nz=0;
  fs->read((char *)&x, sizeof(float));
  fs->read((char *)&y, sizeof(float));
  fs->read((char *)&z, sizeof(float));
  fs->read((char *)&nx, sizeof(float));
  fs->read((char *)&ny, sizeof(float));
  fs->read((char *)&nz, sizeof(float));

  if(fs->eof())return -1;
 
  //  std::cout<<"x = "<<x<<", y = "<<y<<", z = "<<z<<", nx = "<<nx<<", ny = "<<ny<<", nz = "<<nz<<std::endl;
  std::cout<<" "<<x<<" "<<y<<" "<<z<<" "<<nx<<" "<<ny<<" "<<nz<<std::endl;
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
  std::cout<<"      exec_name: -inF CASTOR_data.cdf"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"  options:"<<std::endl;
  std::cout<<" -h                      print this help message"<<std::endl;
  std::cout<<" -inF CASTOR_data.cdf set the name of MC data file (required)"<<std::endl;
  exit(-2);
}
