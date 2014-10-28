

#include "HCNode.hpp"
#include <iostream>

typedef unsigned char byte;

using namespace std;


  /** Less-than comparison, so HCNodes will work in std::priority_queue
   *  We want small counts to have high priority.
   *  And we want to break ties deterministically.
   */
  bool HCNode::operator<(const HCNode& other){
    if(count != other.count){
       return count > other.count;
    }

    return symbol < other.symbol;
  }


bool comp(HCNode* one, HCNode* other){
    return one->symbol > other->symbol;
}

