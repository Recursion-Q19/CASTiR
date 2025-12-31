#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // For strlen

int main()
{
    // Replace these with your actual file names
    std::string file1Name = "test1.clm.safir"; // First file
    std::string file2Name = "test2.clm.safir"; // Second file

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

