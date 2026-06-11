#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // For strlen

std::string file1Name = "";
std::string file2Name = "";

void Usage(void);
void ParseCommandLineArguments(int narg, char* argv[]);

int main(int narg, char* argv[])
{
	ParseCommandLineArguments(narg, argv);
	
    //// Replace these with your actual file names
    //std::string file1Name = "rotated1.clm.safir"; // First file
    //std::string file2Name = "rotated7.clm.safir"; // Second file

    // Open the first file in append mode
    std::ofstream file1(file1Name, std::ios::out | std::ios::app | std::ios::binary);
    if (!file1) {
        std::cerr << "Failed to open " << file1Name << " for appending.\n";
        return 1;
    }

    // Open the second file in read mode
    std::ifstream file2(file2Name, std::ios::in | std::ios::binary);
    if (!file2) {
        std::cerr << "Failed to open " << file2Name << " for reading.\n";
        return 1;
    }

    // Skip the header in the second file
    const char *signature = "SAFIR CListModeData\0";
    const int signatureSize = strlen(signature) + 1; // Including null terminator
    const int additionalHeaderSize = 12; // As per your code
    const int headerSize = signatureSize + additionalHeaderSize; // Total header size

    // Move the read pointer past the header in file2
    file2.seekg(headerSize, std::ios::beg);
    if (!file2) {
        std::cerr << "Failed to skip header in " << file2Name << ".\n";
        return 1;
    }

    // Buffer for data transfer
    const size_t bufferSize = 8192; // 8 KB buffer
    char buffer[bufferSize];

    // Copy the event data from file2 to file1
    while (file2) {
        file2.read(buffer, bufferSize);
        std::streamsize bytesRead = file2.gcount();
        if (bytesRead > 0) {
            file1.write(buffer, bytesRead);
            if (!file1) {
                std::cerr << "Failed to write to " << file1Name << ".\n";
                return 1;
            }
        }
    }

    // Close the files
    file1.close();
    file2.close();

    std::cout << "Files combined successfully.\n";
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
				if (arg=="-inF1")
					{
						file1Name = argv[i+1];
					}
				else
					{
						Usage();
					}
				break;
			case 3:
				if (arg=="-inF2")
					{
						file2Name = argv[i+1];
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

  if(file1Name=="" || file2Name=="")
    {
      Usage();
    }

  std::cout<<" Input file name = " << file1Name <<std::endl;
  std::cout<<" output file name = " << file2Name <<std::endl;
}


//----------------------------
// Usage
//----------------------------
void Usage(void)
{
  std::cout<<std::endl;
  std::cout<<"Usage:"<<std::endl;
  std::cout<<"      exec_name:  -inF1 stir_inData1.clm.safir -inF2 stir_inData2.clm.safir" <<std::endl;
  std::cout<<std::endl;
  std::cout<<"  options:"<<std::endl;
  std::cout<<" -h                      print this help message"<<std::endl;
  std::cout<<" -inF1 stir_inData1.clm.safir set the name of first MC input data file (required)"<<std::endl;
  std::cout<<" -inF2 stir_inData2.clm.safir set the name of second MC input data file (required) which will be appended to the first file"<<std::endl;
  exit(-2);
}
