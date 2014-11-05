#include "BitInputStream.hpp"
#include <iostream>
#include <bitset>

using namespace std;

    /** Read the next bit from the bit buffer.
     *  If the bit buffer is currently empty,
     *  fill the bit buffer by reading a char from the istream first.
     *  Return the bit read as the least signficant bit of an int.
     *  Return -1 on EOF.
     *  This must be consistent with BitOutputStream::writeBit(), in terms
     *  of ordering of bits in the stream.
     */
    int BitInputStream::readBit(){
        int nextBit = 256;
        if(in.eof()){
            return -1;
        }
        if(bufi == 8){
            buf = in.get();
            bufi = 0;
        }

        // getting next bif from buf
        nextBit = nextBit &  buf;
        nextBit = nextBit >> 8; 
        // shifting buff to left to read next bit
        buf = buf << 1;
        bufi++;

        return nextBit;
    }

    /** Read a byte from the ostream.
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading bytes.
     */
    int BitInputStream::readByte(){
        int readChar = in.get();
        return readChar;
    }

    /** Read a non-negative int from the ostream.
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading ints.
     */
    int BitInputStream::readInt(){
        int readInt;
        in.read(reinterpret_cast<char*>(&readInt), sizeof(readInt));
        if(in.eof()){
            return -1;
        }
        return readInt;
    }


