#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Wrong number of arguments, please enter two files" << endl;
    }
    else{
        // Initializing the Bit Input and Output streams
        ifstream inFile;
        istream& constructIn = inFile;
        inFile.open(argv[1], ios::in | ios::binary);
        BitInputStream* in = new BitInputStream(constructIn);

        // Finds the size of the input file
        inFile.seekg(0, std::ios_base::end);
        int fileSize = inFile.tellg();
        inFile.seekg(0, inFile.beg);

        ofstream outFile;
        ostream& construcOut = outFile;
        outFile.open(argv[2], ios::out | ios::binary);
        BitOutputStream* out = new BitOutputStream(construcOut);

        unsigned char readCh; // char that is being read in
        int chIndex; // Position that char is currently on
        int value;   // Value of the next bit
        vector<int> frequency(256, 0); // vector to store the symbol frequency

        // In case the input file does not exist, prints error and returns
        if(!inFile){
            cout << "Input file does not exist! Please try again" << endl;
            return 0;
        }

        // Reads in the file and creates the vector of frequencys
        while(1){
           value = in->readBit();
           readCh |= value;
           // Shifts to the right until reach end of the buffer (index 8)
           if(chIndex != 7){ 
               readCh <<= 1;
           }
           chIndex++;
           if(inFile.eof()){
               break;
           }
           if(chIndex == 8){
               // Increments at the correct index
               frequency[(int)readCh]++;
               // Reset the char and index
               readCh <<= 8;
               chIndex = 0;
           }
        }
        inFile.close();

        // Prints out the vector of frequency as the header
        for(unsigned int i = 0; i < frequency.size(); i++){
            // Writes a single 0 byte if freq is 0
            if(frequency[i] == 0){
                out->writeByte(0);
            } 
            else{
                out->writeInt(frequency[i]);
            }
        }
        // Writes out the size of the file 
        out->writeInt(fileSize);

        // Building the tree using the frequency of symbols
        HCTree* tree = new HCTree();
        tree->build(frequency);

        // Encodes the symbols into binary
        inFile.open(argv[1], ios::in | ios::binary);
        in = new BitInputStream(constructIn);
        chIndex = 0;
        // Reads bits and encodes them once 8 are read in
        while(1){
           value = in->readBit();
           readCh |= value;
           if(chIndex != 7){
               readCh <<= 1;
           }
           chIndex++;
           if(inFile.eof()){
               break;
           }
           if(chIndex == 8){
               // Encodes if the char is in the tree
               if(readCh == tree->leaves[(int)readCh]->symbol){
                   tree->encode(readCh, *out);
               }
               chIndex = 0;
               readCh <<= 8;
           }
        }
        out->flush();
        inFile.close();
        outFile.close();
        
    }
    return 0;
}
