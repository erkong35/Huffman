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
        ifstream inFile;
        istream& constructIn = inFile;
        ofstream outFile;
        ostream& construcOut = outFile;
         
        inFile.open(argv[1], ios::binary);
        BitInputStream* in = new BitInputStream(constructIn);

        outFile.open(argv[2], ios::binary);
        BitOutputStream* out = new BitOutputStream(construcOut);


        char readCh; // char that is being read in
        int chIndex; // Index that char is currently on

        // In case the input file does not exist, prints error and returns
        if(!inFile){
            cout << "Input file does not exist! Please try again" << endl;
            return 0;
        }

        vector<int> frequency(256, 0);
        int value;
        // Reads in the file and creates the vector of frequencys
        while(1){
           value = in->readBit();
           readCh = readCh | value;
           if(chIndex != 7){
               readCh = readCh << 1;
           }
           chIndex++;
           if(inFile.eof()){
               break;
           }
           if(chIndex == 8){
               frequency[(int)readCh]++;
               readCh = readCh << 8;
               chIndex = 0;
           }
        }
        inFile.close();
        // Prints out the vector of frequency as the header
        for(unsigned int i = 0; i < frequency.size(); i++){
            out->writeInt(frequency[i]);
        }

        // Building the tree using the frequency of symbols
        HCTree* tree = new HCTree();
        tree->build(frequency);

        // Encodes the symbols into binary
        inFile.open(argv[1], ios::binary);
        in = new BitInputStream(constructIn);
        chIndex = 0;
        while(1){
           value = in->readBit();
           readCh = readCh | value;
           if(chIndex != 7){
               readCh = readCh << 1;
           }
           chIndex++;
           if(inFile.eof()){
               out->flush();
               break;
           }
           if(chIndex == 8){
               if(readCh == tree->leaves[(int)readCh]->symbol){
                   tree->encode(readCh, *out);
               }
               chIndex = 0;
               readCh = readCh << 8;
           }
        }
        inFile.close();
        outFile.close();
        
    }
    return 0;
}
