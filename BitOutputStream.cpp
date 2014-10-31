#include "BitOutputStream.hpp"
#include <iostream>
#include <bitset>

using namespace std;

  /** Write the least significant bit of the argument into
   *  the bit buffer, and increment the bit buffer index.
   *  Flush to the ostream first if the bit buffer is full.
   *  This must be consistent with BitInputStream::readBit().
   */
  void BitOutputStream::writeBit(int bit){
      if(bufi == 8){
          flush();
      }

      // Shift buffer right to add in bit
      int lsb = bit & 1;
      buf = buf << 1;
      buf = lsb | buf;
      bufi++;
  }

  /** Write the least significant byte of the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing bytes.
   */
  void BitOutputStream::writeByte(int b){
      out.put(b);
      out.flush();
  }

  /** Write the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing ints.
   */
  void BitOutputStream::writeInt(int i){
      out.write(reinterpret_cast<const char*>(&i), sizeof(i)); 
  }

  /** If the bit buffer contains any bits, flush the bit buffer to the ostream,
   *  clear the bit buffer, and set the bit buffer index to 0.
   *  Also flush the ostream itself.
   */
  void BitOutputStream::flush(){
          out.put(buf);
          out.flush();
          buf = bufi = 0;
  }
