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
        // Create the Bit Input and Output streams
        ifstream inFile;
        istream& constIStream = inFile;
        inFile.open(argv[1], ios::binary);
        BitInputStream* in = new BitInputStream(constIStream);
        

        ofstream outFile;
        ostream& constOStream = outFile;
        outFile.open(argv[2], ios::binary);
        BitOutputStream* out = new BitOutputStream(constOStream);

        vector<int> frequency(256, 0); // vector to store the symbol frequency

        // Checks if input exists and prints error if it does not
        if(!inFile){
            cout << "Input file does not exist, please try again!" << endl;
            return 0;
        }


        // Creates the frequency vector by reading the header
        for(unsigned int i = 0; i < frequency.size(); i++){
            frequency[i] = in->readInt();
        }
        
        // Building the tree using the frequencies
        HCTree* tree = new HCTree();
        tree->build(frequency);
        // Decodes the compressed files and writes to the output file
        int val; // val from decoding
        while(1){
           val = tree->decode(*in);
           if(val != -1){ 
               out->writeByte((char)val);
           }
           // Reach EOF
           else {
               break;
           }
        }
        inFile.close();
        outFile.close();
    }
}
