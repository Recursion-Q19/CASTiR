#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int readCASToR_HistData(fstream *fs);
void writeCombinedHists(unsigned long int time, fstream *fs);

map<set<uint16_t>, uint16_t> histData;

int main(int narg, char* argv[]){

	//parseTerminalArguments(narg, argv);
	
	unsigned long int event_number = 0;
	if(narg < 5 || string(argv[1]) != "-outF" || string(argv[3]) != "-inF"){
		cout << "Usage: $ " << argv[0] << " -outF output_file_name.cdf -inF input_data1.cdf" << endl;
		exit(-2);
	}
	
	cout << "Filling HistData with Zero!" << endl;
	
	for (uint16_t index = 0; index < 19600; index++){
		for (uint16_t item = index + 1; item < 19600; item++){
			histData[{index, item}] = 0;
		}
	}
		
	cout << "HistData filled with Zero!" << endl;
	
	string inFileName = argv[4];
	
	cout << "InputFile name: " << inFileName << endl; 
	
	fstream inFile;
	
	inFile.open(inFileName.data(), ios::in | ios::binary);//or inFile.open(inFileName, ios::in | ios::binary);
		
		if(!inFile.is_open()){
			cerr << "Failed to open: " << inFileName << endl;
		}
		while(inFile.good()){
			readCASToR_HistData(&inFile);	
		}
		
		inFile.close();
		

		cout << "Number of recorded lines: " << histData.size() << endl;
		
		if (!histData.empty()) {
			auto maxIt = std::max_element(histData.begin(), histData.end(),
			[](const auto& a, const auto& b) {
				return a.second < b.second;  // compare by value (the int)
			});
			
			// Access the result
			set<uint16_t> maxKey   = maxIt->first;
			uint16_t      maxValue = maxIt->second;
			
			cout << "Max value: " << maxIt->second << endl;
			cout << "Key set: {";
			for (auto it = maxIt->first.begin(); it != maxIt->first.end(); ++it) {
				if (it != maxIt->first.begin()) cout << ", ";
				cout << *it;
			}
			cout << "}" << endl;
		}
	
	string outputName = argv[2];
	cout << "Output file opening: " << outputName << " ..." << endl;
	fstream outFile;
	outFile.open(outputName, ios::out | ios::binary); 
	
	if(!outFile.is_open()){
		cerr << "Failed to open: " << outputName << endl;
	}
	
	writeCombinedHists(1000000, &outFile);	
	
	outFile.close();
	
	
	cout << "Number of recorded lines (i.e, (id1, id2) Pairs): " << histData.size() << endl;
	cout << "This value should be used in the header file!!!" << endl;
	
	return 0;
}

//===============================================
// reading data from binary file
//===============================================
int readCASToR_HistData(fstream *fs){

	unsigned int t = 0;
	float numCoincidences = 0;
	unsigned int crystalID11 = 0;
	unsigned int crystalID22 = 0;
	
	if((fs->read((char *)&t, sizeof(unsigned int))) && (fs->read((char *)&numCoincidences, sizeof(float))) && 
		(fs->read((char *)&crystalID11, sizeof(unsigned int))) && (fs->read((char *)&crystalID22, sizeof(unsigned int)))){
	
	//cout << numCoincidences << " : crystalID1: " << crystalID11 << " crystalID2: " << crystalID22 << endl;

		uint16_t crystalID1 = static_cast<uint16_t>(crystalID11);
		uint16_t crystalID2 = static_cast<uint16_t>(crystalID22);
		uint16_t numCoincidence = static_cast<uint16_t>(numCoincidences);

	//cout << numCoincidence << " : crystalID1: " << crystalID1 << " crystalID2: " << crystalID2 << endl;

		histData[{crystalID1, crystalID2}] = histData[{crystalID1, crystalID2}] + numCoincidence;
	//cout << "crystalID1: " << crystalID1 << " - crystalID2: " << crystalID2 << " - histData[{crystalID1, crystalID2}]: " << histData[{crystalID1, crystalID2}] << endl;
	}
	
	return 0;
}

//===============================================
// Writing data to binary file
//===============================================

void writeCombinedHists(unsigned long int time,fstream *fs)
	{ 
		double temp = time/1000000.;
	  	temp *=4;
	  	unsigned int t = temp;
		float temp1;
		unsigned int temp2;

		for(std::map<std::set<uint16_t>, uint16_t>::iterator it_map = histData.begin(); it_map != histData.end(); it_map++){

			fs->write((char *)&t, sizeof(unsigned int));
				
			temp1 = static_cast<float>(it_map->second);
			
			fs->write((char *)&temp1, sizeof(float));

			for(std::set<uint16_t>::iterator it_set=(it_map->first).begin(); it_set != (it_map->first).end(); it_set++){
				temp2 = static_cast<unsigned int>(*it_set);
				fs->write((char *)&(*it_set), sizeof(unsigned int));
			}
		}
	}

