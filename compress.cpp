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
        vector<int> frequency = vector<int>(256, 0);
        ifstream inFile;
        ofstream outFile;
        inFile.open(argv[1]);
        outFile.open(argv[2]);
        while(!inFile.eof()){
           int value = (int)inFile.get();
           frequency[value] = frequency[value] + 1;
        }
        inFile.close();
        HCTree* tree = new HCTree();
        tree->build(frequency);
        inFile.open(argv[1]);
        while(!inFile.eof()){
           tree->encode(inFile.get(), outFile);
        }
        inFile.close();
        outFile.close();
    }
}
