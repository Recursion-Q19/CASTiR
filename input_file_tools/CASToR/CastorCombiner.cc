#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: CastorCombiner file1.cdf file2.cdf output.cdf\n";
        return 1;
    }

    const char* inputFileName1 = argv[1];
    const char* inputFileName2 = argv[2];
    const char* outputFileName = argv[3];

    // Open the first input file
    std::ifstream inputFile1(inputFileName1, std::ios::in | std::ios::binary);
    if (!inputFile1)
    {
        std::cerr << "Error opening input file: " << inputFileName1 << "\n";
        return 1;
    }

    // Open the second input file
    std::ifstream inputFile2(inputFileName2, std::ios::in | std::ios::binary);
    if (!inputFile2)
    {
        std::cerr << "Error opening input file: " << inputFileName2 << "\n";
        return 1;
    }

    // Open the output file
    std::ofstream outputFile(outputFileName, std::ios::out | std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error opening output file: " << outputFileName << "\n";
        return 1;
    }

    // Variables to hold the counts of records
    int count1 = 0;       // Number of records in the first input file
    int count2 = 0;       // Number of records in the second input file
    int totalCount = 0;   // Total number of records in the output file

    // Helper function to copy data from an input file to the output file
    auto copyData = [](std::ifstream& inFile, std::ofstream& outFile) -> int {
        unsigned int t_local, ID1_local, ID2_local;
        int recordCount = 0;
        while (inFile.read(reinterpret_cast<char*>(&t_local), sizeof(unsigned int)))
        {
            if (!inFile.read(reinterpret_cast<char*>(&ID1_local), sizeof(unsigned int)))
                break;
            if (!inFile.read(reinterpret_cast<char*>(&ID2_local), sizeof(unsigned int)))
                break;

            outFile.write(reinterpret_cast<char*>(&t_local), sizeof(unsigned int));
            outFile.write(reinterpret_cast<char*>(&ID1_local), sizeof(unsigned int));
            outFile.write(reinterpret_cast<char*>(&ID2_local), sizeof(unsigned int));

            recordCount++;
        }
        return recordCount;
    };

    // Copy data from the first input file and get the number of records
    count1 = copyData(inputFile1, outputFile);

    // Copy data from the second input file and get the number of records
    count2 = copyData(inputFile2, outputFile);

    // Calculate the total number of records in the output file
    totalCount = count1 + count2;

    // Close all files
    inputFile1.close();
    inputFile2.close();
    outputFile.close();

    // Display the counts
    std::cout << "Files have been successfully merged into " << outputFileName << "\n";
    std::cout << "Number of records in " << inputFileName1 << ": " << count1 << "\n";
    std::cout << "Number of records in " << inputFileName2 << ": " << count2 << "\n";
    std::cout << "Total number of records in " << outputFileName << ": " << totalCount << "\n";

    return 0;
}

