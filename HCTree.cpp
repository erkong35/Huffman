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
        for(int i = 0; i < leaves.size(); i++){
            if(freqs[i] != 0){
                HCNode* temp = new HCNode(freqs[i], i);
                leaves[i] = temp;
            }
        }

        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
        for(int j = 0; j < leaves.size(); j++){
            if(leaves[j] != 0){
                pq.push(leaves[j]);
            }
        }
        
        if(pq.size() == 1){
            HCNode* onlyChild = pq.top();
            int childI = 0;
            for(int i = 0; i < leaves.size(); i++){
                if(leaves[i] != 0 && leaves[i]->symbol == onlyChild->symbol){
                    childI =  i;
                }
            }
            pq.pop();
            HCNode* newParent = new HCNode(onlyChild->count, 
                                           onlyChild->symbol,
                                           leaves[childI]);
            leaves[childI]->p = newParent;
            pq.push(newParent);
            root = pq.top();
            pq.pop();
            return;
        }

        while(pq.size() > 0){
            HCNode* smallest = pq.top();
            int smallI, secSmallI;
            for(int i = 0; i < leaves.size(); i++){
                if(leaves[i] != 0 && leaves[i]->symbol == smallest->symbol){
                    smallI =  i;
                }
            }
            pq.pop();
            HCNode* secSmallest = pq.top();
            for(int i = 0; i < leaves.size(); i++){
                if(leaves[i] != 0 && leaves[i]->symbol == secSmallest->symbol){
                    secSmallI = i;
                }
            }
            pq.pop();
            if(comp(smallest, secSmallest)){
                HCNode* newParent = new HCNode(smallest->count + 
                                               secSmallest->count, 
                                               secSmallest->symbol,
                                               leaves[smallI],
                                               leaves[secSmallI]);  
                leaves[smallI]->p = newParent;
                leaves[secSmallI]->p = newParent;
                pq.push(newParent);
            }
            else{ 
                HCNode* newParent = new HCNode(smallest->count + 
                                               secSmallest->count, 
                                               smallest->symbol,
                                               leaves[secSmallI], 
                                               leaves[smallI]); 
                leaves[smallI]->p = newParent;
                leaves[secSmallI]->p = newParent;
                pq.push(newParent);
            }
        }
        root = pq.top();
        pq.pop();
    }
           
    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void HCTree::encode(byte symbol, BitOutputStream& out) const{}

    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
    void HCTree::encode(byte symbol, ofstream& out) const{
        vector<int> encodedSymbol;
        int i = 0;
        for(int j = 0; j < leaves.size(); j++){
            if(leaves[j] != 0 && leaves[j]->symbol == symbol){
                i = j;
                break;
            }
        }
        
        HCNode* tmp = leaves[i];

        if(leaves[i] == 0){
            return;
        }
        while(tmp->p != NULL){
            if(tmp->p->c0 == tmp){
                encodedSymbol.push_back(0);
            }
            else{
                encodedSymbol.push_back(1);
            }
            tmp = tmp->p; 
        }

        reverse(encodedSymbol.begin(), encodedSymbol.end());
        for(int j = 0; j < encodedSymbol.size(); j++){
            out << encodedSymbol[j];
        }
    }


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int HCTree::decode(BitInputStream& in) const{}

    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    int HCTree::decode(ifstream& in) const{
        HCNode* tmp = root;
        while(tmp->c0 != NULL && tmp->c1 != NULL){
            int tempBit = in.get();
            if(tempBit == 0){
                tmp = tmp->c0;
            }
            else{
                tmp = tmp->c1;
            }
        }
        return tmp->symbol;
    }


