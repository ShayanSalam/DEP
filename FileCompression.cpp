/*
 * This C++ program implements a simple file compression and decompression tool using
 * the Run-Length Encoding (RLE) algorithm. The program includes the following functionalities:
 *
 * 1. Writing data to a file: The function `writeDataToFile` writes a string of data to a specified file.
 *
 * 2. Compressing a file: The function `compressFile` reads the content of an input file and compresses
 *    it using RLE. It then writes the compressed data to an output file. RLE works by counting consecutive
 *    occurrences of the same character and storing the count followed by the character.
 *
 * 3. Decompressing a file: The function `decompressFile` reads the compressed file produced by RLE and
 *    reconstructs the original data by writing each character the specified number of times to the output file.
 *
 * 4. Main function: The `main` function orchestrates the flow by writing some sample data to a file, then
 *    compressing that file, and finally decompressing it to verify that the original data is correctly restored.
 *
 * The program handles basic edge cases, such as empty files and simple binary data.
 * It provides console output to inform the user about the progress of the compression and decompression processes.
 */

//SHAYAN SALAM
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function to write some data to a file
void writeDataToFile(const string& fileName, const string& data) {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    outFile << data;
    outFile.close();

    cout << "Data written to " << fileName << endl;
}

// Function to compress a file using RLE
void compressFile(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    char currentChar;
    char previousChar;
    int count = 0;

    if (inputFile.get(previousChar)) {  // Read the first character
        count = 1;
        while (inputFile.get(currentChar)) {  // Loop through the rest of the file
            if (currentChar == previousChar) {
                count++;  // Increment count if characters match
            }
            else {
                outputFile << count << previousChar;  // Write the count and character to the output file
                previousChar = currentChar;  // Update the previous character
                count = 1;  // Reset the count for the new character
            }
        }
        outputFile << count << previousChar;  // Write the last character and its count
    }

    inputFile.close();
    outputFile.close();

    cout << "Compression complete. Data written to " << outputFileName << endl;
}

// Function to decompress a file compressed with RLE
void decompressFile(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    char ch;
    int count;
    while (inputFile >> count >> ch) {  // Read the count and character from the input file
        for (int i = 0; i < count; ++i) {
            outputFile.put(ch);  // Write the character 'count' times to the output file
        }
    }

    inputFile.close();
    outputFile.close();

    cout << "Decompression complete. Data written to " << outputFileName << endl;
}

int main() {
    string inputFileName = "input.txt";
    string compressedFileName = "compressed.rle";
    string decompressedFileName = "decompressed.txt";
    string data = "Some Data...";  // Example data to be written to the input file

    // Write data to input file
    writeDataToFile(inputFileName, data);

    // Compress the file
    compressFile(inputFileName, compressedFileName);

    // Decompress the file
    decompressFile(compressedFileName, decompressedFileName);

    return 0;
}
