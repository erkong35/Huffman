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
        HCNode* freqSum;

        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
        for(int i = 0; i < leaves.size(); i++){
            if(freqs[i] != 0){
                HCNode* temp = new HCNode(freqs[i], (byte)i);
                leaves[i] = temp;
                pq.push(temp);
            }
        }

/*        for(int j = 0; j < leaves.size(); j++){
            if(leaves[j] != 0){
                pq.push(leaves[j]);
            }
        }*/
        if(pq.size() == 0){
            return;
        }
        while(pq.size() > 1){
            smallest = pq.top();
            pq.pop();
            secSmallest = pq.top();
            pq.pop();
            freqSum = new HCNode(smallest->count + secSmallest->count,
                                 (char) 33);
            pq.push(freqSum);
            smallest->p = freqSum;
            secSmallest->p = freqSum;
            freqSum->c0 = secSmallest;
            freqSum->c1 = smallest; 
        }
        root = freqSum;

/*        if(pq.size() == 1){
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
            pq.push(newParent);
            leaves[childI]->p = newParent;
            root = pq.top();
            pq.pop();
            return;
        }
        HCNode* smallest;
        HCNode* secSmallest;
        while(pq.size() > 1){
            smallest = pq.top();
            int smallI, secSmallI;
            for(int i = 0; i < leaves.size(); i++){
                if(leaves[i] != 0 && leaves[i]->symbol == smallest->symbol){
                    smallI =  i;
                }
            }
            pq.pop();
            secSmallest = pq.top();
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
                cout << "Side1" << endl;
            cout << (char)newParent->symbol<<endl;
            cout << (char)newParent->c0->symbol<<endl;
            cout << (char)newParent->c1->symbol<<endl;

                pq.push(newParent);
            }
            else{ 
                HCNode* newParent = new HCNode(smallest->count + 
                                               secSmallest->count, 
                                               smallest->symbol,
                                               leaves[secSmallI], 
                                               leaves[smallI]); 
                pq.push(newParent);
                cout << "Side2" << endl;
            cout << (char)newParent->c0->symbol<<endl;
            cout << (char)newParent->c1->symbol<<endl;
                leaves[smallI]->p = newParent;
                leaves[secSmallI]->p = newParent;
            }
        }
        cout << "root" << endl;
        root = pq.top();
        cout << (char)root->symbol << endl;
        cout << (char)root->c0->symbol << endl;
        cout << (char)root->c1->symbol << endl;
        cout << (char)root->c0->c0->symbol << endl;
        cout << (char)root->c0->c1->symbol << endl;

        pq.pop();*/
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
        char ch;
        while(1){
            ch = in.get();
            if(! in.good()){
                return -1;
            }
            if(ch == '1'){
                if(tmp->c1 != NULL){
                    tmp = tmp->c1;
                }
                if(tmp->c1 == NULL){
                    return tmp->symbol;
                }
            }
            else if(ch == '0'){
                if(tmp->c0 != NULL){
                    tmp = tmp->c0;
                }
                if(tmp->c0 == NULL){
                    return tmp->symbol;
                }
            }
          //  if(tempBit == 0 && tmp->c0 != NULL){
          //      tmp = tmp->c0;
          //  }
          //  else if(tempBit == 1 && tmp->c1 != NULL){
          //      tmp = tmp->c1;
          //  } 
          //  else {
          //      break;
          //  }
        }
    }


