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
        outFile.open(argv[2], ios::binary);
        string freqStr;
        int value;

        for(int i = 0; i < frequency.size(); i++){
            inFile >> value;
            frequency[i] = value;
        }
        
        HCTree* tree = new HCTree();
        tree->build(frequency);
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
