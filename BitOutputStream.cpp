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

      // Shift buffer left to add in bit
      buf = buf << 1;
      buf = bit | buf;
      bufi++;
  }

  /** Write the least significant byte of the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing bytes.
   */
  void BitOutputStream::writeByte(int b){
      unsigned char c = static_cast<unsigned char>(b);
      out.put(c);
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
      if(bufi != 0){
          buf = buf << (8-bufi);
          out.put(buf);
          buf = bufi = 0;
          out.flush();
      }
  }
