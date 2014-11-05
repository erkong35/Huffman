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
        inFile.open(argv[1], ios::in | ios::binary);
        BitInputStream* in = new BitInputStream(constIStream);
        

        ofstream outFile;
        ostream& constOStream = outFile;
        outFile.open(argv[2], ios::out | ios::binary);
        BitOutputStream* out = new BitOutputStream(constOStream);

        int fileSize;  // Size of original file so uncompress does not decode
                       // too much.
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
        fileSize = in->readInt();

        // Building the tree using the frequencies
        HCTree* tree = new HCTree();
        tree->build(frequency);

        int val; // val from decoding
        int counter = 0; // Stops the while loop when this hits the file size

        // Decodes the compressed files and writes to the output file
        while(counter < fileSize){
           val = tree->decode(*in);
           out->writeByte((char)val);
           counter++;
        }
        inFile.close();
        outFile.close();
    }
}
