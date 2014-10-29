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
        vector<int> frequency(256, 0);
        ifstream inFile;
        ofstream outFile;
        inFile.open(argv[1], ios::binary);
        outFile.open(argv[2], ios::binary);
        int value;
        // Reads in the file and creates the vector of frequencys
        while(1){
           value = (int)inFile.get();
           if(inFile.eof()){
               break;
           }
           frequency[value]++;
        }
        inFile.close();

        // Prints out the vector of frequency as the header
        for(unsigned int i = 0; i < frequency.size(); i++){
            outFile << (frequency[i]);
            outFile << " ";
        }

        // Building the tree using the frequency of symbols
        HCTree* tree = new HCTree();
        tree->build(frequency);

        // Encodes the symbols into binary
        inFile.open(argv[1], ios::binary);
        while(1){
           byte symb = inFile.get();
           if(inFile.eof()){
               break;
           }
           tree->encode(symb, outFile);
        }
        inFile.close();
        outFile.close();
    }
    return 0;
}
