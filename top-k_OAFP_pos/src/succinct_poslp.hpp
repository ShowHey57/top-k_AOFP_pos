/* succinct_poslp.hpp
MIT License

Copyright (c) 2017 Yoshimasa Takabatake

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SUC_POSLP_HPP_
#define SUC_POSLP_HPP_

#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "constant_numbers.hpp"
#include "suc_fbtree.hpp"
#include "inner_leaf.hpp"
#include "out_leaf.hpp"
#include "freq_crdict.hpp"
#include "control_pq.hpp"

namespace comp {

  //node of POSLP before encoding
  class Node{
  public:
    uint64_t symbol;
    bool is_inner;
    uint64_t _len;
    uint64_t _pos;//pos
  public:
    Node(): symbol(kDummyCode),
	    is_inner(false),
      _len(0),
      _pos(kDummyCode){}
    Node(const uint8_t kChar, const uint64_t kPos): symbol(kChar),
			       is_inner(false),
             _pos(kPos),
             _len(1){}
    void Set(const uint64_t kSymbol, const bool kIsInner){
      symbol = kSymbol;
      is_inner = kIsInner;
    }
    void Set(const uint64_t kSymbol, const bool kIsInner, const uint64_t len){
      symbol = kSymbol;
      is_inner = kIsInner;
      _len = len;
    }

    void Set(const uint64_t kSymbol, const bool kIsInner,const uint64_t len, const uint64_t pos){
      symbol = kSymbol;
      is_inner = kIsInner;
      _len = len;
      _pos = pos;

    }
  }; // class Node

  //the succinct data structure of post-order straight line program
  //this supports access/reverse access of production rules and pushing back
  // a new character in O(\lg\lg n) time
  //using n\lg (n + \sigma) + o(n\n\lg(n + \sigma)) bits of space.
  class SucPOSLP {
  private:
    //sunccinct data structure 
    SucFBTree sfbt_; 
    InnerLeaf inner_leaf_;
    OutLeaf outer_leaf_;
    //
    //buffer for this online construction 
    std::vector<Node> buf_;
    uint64_t buf_size_;
    // # of production rules
    uint64_t num_rules_;
    //output file name
    std::string output_file_name_;
    //data structure for reverse acessing to frequent production rules.
    FreqCRDict  freq_crdict_;

    
  public:
    //constructor and destructor
    SucPOSLP(): sfbt_(), 
		inner_leaf_(),
		outer_leaf_(),
		buf_(),
		buf_size_(),
		num_rules_(0),
		output_file_name_(),
		freq_crdict_(){};
    ~SucPOSLP() {};

    Control_Pq cpd;

    //initialization and clear of data structures
    void Init(const std::string &kOutputFileName);
    void Clear();

    //accessor
    uint64_t NumRules() const;

    //pushing back a new symbol to succint POSLP 
    inline void PushToBuffer(const Node &kNode);
    void PushLastCP();
    void     PushBuffers(const uint64_t kBufSize);
    uint64_t PushInnerSFBT();
    uint64_t PushOuterSFBT();
    
    //accessing POSLP's tree 
    uint64_t Left(const uint64_t kVar);
    uint64_t Right(const uint64_t kVar);
    uint64_t ParentOfInNode(const uint64_t kVar, 
			    bool &is_left);


    //reverse access and update POSLP
    Node     ReverseAccessAndUpdate(const Node   &kLeft, 
				    const Node   &kRight);

    //space infromation
    uint64_t ByteSizeOfB() const;
    uint64_t ByteSizeOfL1() const;
    uint64_t ByteSizeOfL2L3() const;
    uint64_t ByteSizeOfL() const;
    uint64_t ByteSizeOfPOSLP() const;
    uint64_t ByteSizeOfOther() const;
    uint64_t ByteSizeOfHash() const;
    uint64_t ByteSize() const;

    //save and load
    void Save(std::ofstream &ofs);
    void Load(std::ifstream &ifs);

    void PrintTree(const Node &kNode);;
  
  private:
    //decompression method
    void Decode(const uint64_t kVar,
		std::ofstream &ofs2);
    // transformation of the variable of post order straight line program
    //to the variable of succinct full binary tree
    // and the reversed transformation
    inline uint64_t Var2SFBTVar(const uint64_t kVar);
    inline uint64_t SFBTVar2Var(const uint64_t kVar);
  }; // class SucPOSLP
  //inline implementations
  inline uint64_t SucPOSLP::Var2SFBTVar(const uint64_t kVar) {
    if (kVar < kAlphabetSize){
      return kDummyCode;
    }
    return kVar - kAlphabetSize;
  }

  inline uint64_t SucPOSLP::SFBTVar2Var(const uint64_t kVar) {
    return kVar + kAlphabetSize;
  }

  inline void SucPOSLP::PushToBuffer(const Node &kNode) {
    if (kNode.symbol >= kAlphabetSize) {
      buf_.pop_back();
      buf_.pop_back();
      buf_size_ -= 2;
    }
    buf_.push_back(kNode);
    ++buf_size_;
  }

} // namespace comp

#endif // SUC_POSLP_HPP_

