#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
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

        // Checks if input exists and prints error if it does not
        if(!inFile){
            cout << "Input file does not exist, please try again!" << endl;
            return 0;
        }

        outFile.open(argv[2], ios::binary);

        // Creates the frequency vector by reading the header
        int value;
        for(unsigned int i = 0; i < frequency.size(); i++){
            inFile >> value;
            frequency[i] = value;
        }
        
        // Building the tree using the frequencies
        HCTree* tree = new HCTree();
        tree->build(frequency);

        // Decodes the compressed files and writes to the output file
        int val;
        while(1){
           val = tree->decode(inFile);
           if(val != -1){    
               outFile << (char)val;
           }
           else {
               break;
           }
        }
        inFile.close();
        outFile.close();
    }
    return 0;
}
