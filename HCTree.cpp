#include <queue>
#include <vector>
#include <fstream>
#include <algorithm>
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

    HCTree::~HCTree(){}

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void HCTree::build(const vector<int>& freqs){
        HCNode* smallest;
        HCNode* secSmallest;
        HCNode* tmpParent;
        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

        // Adds nodes to the leaves vector and the pq for sorting
        for(unsigned int i = 0; i < leaves.size(); i++){
            if(freqs[i] != 0){
                HCNode* temp = new HCNode(freqs[i], (byte)i);
                leaves[i] = temp;
                pq.push(temp);
            }
        }

        // Handles when there are no symbols (empty txt file) just returns
        if(pq.size() == 0){
            return;
        }

        // Handles when there is only 1 symbol
        if(pq.size() == 1){
            root = pq.top();
            pq.pop();
            return;
        }

        // Handles when there are 2 or more symbols
        while(pq.size() > 1){
            smallest = pq.top();
            pq.pop();
            secSmallest = pq.top();
            pq.pop();

            // Smaller symbol propagates its character to the parent
            if(comp(smallest, secSmallest)){
                tmpParent = new HCNode(smallest->count + secSmallest->count,
                                       (char) secSmallest->symbol);
            }
            else {
                tmpParent = new HCNode(smallest->count + secSmallest->count,
                                       (char) smallest->symbol);
            }

            pq.push(tmpParent);
            smallest->p = tmpParent;
            secSmallest->p = tmpParent;
            tmpParent->c0 = secSmallest;
            tmpParent->c1 = smallest; 
        }
        root = tmpParent;
    }

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void HCTree::encode(byte symbol, BitOutputStream& out) const{
        vector<int> encodedSymbol;
        int i = 0;
        // Finds the index for the symbol in leaves
        for(unsigned int j = 0; j < leaves.size(); j++){
            if((leaves[j] != 0) && (leaves[j]->symbol == symbol)){
                i = j;
                break;
            }
        }
        
        HCNode* tmp = leaves[i];

        // Case where root is one node
        if(tmp->p == NULL && tmp->c0 == NULL && tmp->c1 == NULL){
            out.writeBit(0);
            return;
        }

        // Creates the path for the leaf
        while(tmp->p != NULL){
            if(tmp->p->c0 == tmp){
                encodedSymbol.push_back(0);
            }
            else{
                encodedSymbol.push_back(1);
            }
            tmp = tmp->p; 
        }

        // Reverses the path because we started from the bottom
        reverse(encodedSymbol.begin(), encodedSymbol.end());
        for(unsigned int j = 0; j < encodedSymbol.size(); j++){
            out.writeBit(encodedSymbol[j]);
        }
    }


    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
//    void HCTree::encode(byte symbol, ofstream& out) const{}


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int HCTree::decode(BitInputStream& in) const{
        HCNode* tmp = root;
        int i;
        
        // Only root tree
        if(tmp != 0 && tmp->c0 == NULL && tmp->c1 == NULL){
            i = in.readBit();
            if(i == -1){
                return -1;
            }
            return tmp->symbol;
        }

        while(1){
            i = in.readBit();
            // Traverses the tree until the correct symbol is reached
            if(i == 1){
                if(tmp->c1 != NULL){
                    tmp = tmp->c1;
                }
                if(tmp->c1 == NULL){
                    return tmp->symbol;
                }
            }
            else if(i == 0){
                if(tmp->c0 != NULL){
                    tmp = tmp->c0;
                }
                if(tmp->c0 == NULL){
                    return tmp->symbol;
                }
            }
        }
    }


    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
//    int HCTree::decode(ifstream& in) const{}

