/* solca.cpp
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

#include "solca.hpp"
using namespace std;

namespace comp {

  void SOLCA::Init(const string& kOutputFileName) {
    sposlp_.Init(kOutputFileName);
    CFunc::ResizeVec(ques_,
		     kMaxNumQue);
    CFunc::ResizeVec(que_len_,
		     kMaxNumQue);
    max_level_ = 0;
    for (uint64_t i = 0; i != kMaxNumQue; ++i) {
    	for(uint64_t j = 0; j < 3; j++){
			ques_[i].push_back(Node());
    	}
    	que_len_[i] = 3;
    }
  }

  bool SOLCA::HasLandmark(const uint64_t kLevel) const {
    if (IsRepetition(kLevel, 1)){
      return true;
    }
    else if (IsRepetition(kLevel, 2)){
      return false;
    }
    else if (IsRepetition(kLevel, 3)){
      return true;
    }
    else if (IsRepetition(kLevel, 0)){
      return false;
    }
    else if (IsMaximal(kLevel, 2)){
      return false;
    }
    return true;
  }

  void SOLCA::BuildPOSLP(const Node& kNode,
			 const uint64_t kLevel) {
    ques_[kLevel].push_back(kNode);
    que_len_[kLevel]++;
    if(max_level_ < kLevel){
      max_level_++;
    }
    Node tmp_node;
    if (que_len_[kLevel] == 5) {
    	if (HasLandmark(kLevel)) {
        PopFrontQue2(kLevel);
        tmp_node = sposlp_.ReverseAccessAndUpdate(ques_[kLevel][1], ques_[kLevel][2]);
        BuildPOSLP(tmp_node, kLevel + 1);
        sposlp_.cpd.SetPos(tmp_node.symbol, tmp_node._pos);
        /*if(sposlp_.cpd.MaxVar() < tmp_node.symbol ){
          sposlp_.cpd.SetMax(tmp_node.symbol);
        }else{
        //if(sposlp_.cpd.IsFreq(tmp_node.symbol)){
          sposlp_.cpd.Count(tmp_node.symbol, tmp_node._len, tmp_node._pos);
        }*/
      }
    }
    else if (que_len_[kLevel] == 6) {
    	PopFrontQue3(kLevel);
    	Node middle = sposlp_.ReverseAccessAndUpdate(ques_[kLevel][1], ques_[kLevel][2]);
      sposlp_.cpd.SetPos(middle.symbol, middle._pos);
      //if(sposlp_.cpd.IsFreq(middle.symbol)){
      /*if(sposlp_.cpd.MaxVar() < middle.symbol ){
          sposlp_.cpd.SetMax(middle.symbol);
        }else{
        sposlp_.cpd.Count(middle.symbol, middle._len, middle._pos);
      }*/
    	tmp_node = sposlp_.ReverseAccessAndUpdate(ques_[kLevel][0], middle);
    	BuildPOSLP(tmp_node, kLevel + 1);
      sposlp_.cpd.SetPos(tmp_node.symbol, tmp_node._pos);
      /*if(sposlp_.cpd.MaxVar() < tmp_node.symbol ){
          sposlp_.cpd.SetMax(tmp_node.symbol);
        }else{
      //if(sposlp_.cpd.IsFreq(tmp_node.symbol)){
        sposlp_.cpd.Count(tmp_node.symbol, tmp_node._len, tmp_node._pos);
      }*/
    }
  }

  void SOLCA::ProcessLastSymbols(const uint64_t kCnt,
				 const double kStart) {
    for (uint64_t level = 0; level < kMaxNumQue; ++level) {
      PopFrontQue3(level);
      if (que_len_[level] == 1) {
        if (ques_[level][0].symbol == sposlp_.NumRules() - 1 && max_level_ == level)
          break;
        BuildPOSLP(ques_[level][0], level + 1);
      }
      else if (que_len_[level] == 2) {
      	Node tmp_node;
      	tmp_node = sposlp_.ReverseAccessAndUpdate(ques_[level][0], ques_[level][1]);
        BuildPOSLP(tmp_node, level + 1);
        sposlp_.cpd.SetPos(tmp_node.symbol, tmp_node._pos);
        /*if(sposlp_.cpd.MaxVar() < tmp_node.symbol ){
          sposlp_.cpd.SetMax(tmp_node.symbol);
        }else{
        //if(sposlp_.cpd.IsFreq(tmp_node.symbol)){
          sposlp_.cpd.Count(tmp_node.symbol, tmp_node._len, tmp_node._pos);
        }*/
      }
    }
    //PrintLogs(kCnt, GetTimeOfDaySec() - kStart);
    sposlp_.PushLastCP();
  }

  uint64_t  SOLCA::BSpace() const{
    return sposlp_.ByteSizeOfB();
  }

  uint64_t  SOLCA::L1Space() const{
    return sposlp_.ByteSizeOfL1();
  }

  uint64_t  SOLCA::L2L3Space() const{
    return sposlp_.ByteSizeOfL2L3();
  }

  uint64_t  SOLCA::LSpace() const{
    return sposlp_.ByteSizeOfL();
  }

  uint64_t  SOLCA::HashSpace() const{
    return sposlp_.ByteSizeOfHash();
  }

  uint64_t SOLCA::OtherSpace() const{
    return sposlp_.ByteSizeOfOther()
      + ques_.size() * (sizeof(std::deque<Node>)
			+ sizeof(Node) * kMaxQueLen)
      + kMaxNumQue * sizeof(uint64_t)
      + sizeof(SOLCA);
  }

  uint64_t SOLCA::Space() const{
    return sposlp_.ByteSize()
      + kMaxNumQue * (sizeof(std::deque<Node>)
		      + sizeof(Node) * kMaxQueLen)
      + kMaxNumQue * sizeof(uint64_t)
      + sizeof(SOLCA);
  }

  uint64_t SOLCA::NumRules() const{
    return sposlp_.NumRules();
  }

  uint64_t SOLCA::DictNumRules() const{
    return NumRules();
  }

  uint64_t SOLCA::WriteSize() const{
    return 0;
  }

  void SOLCA::PrintLogs(const uint64_t kCnt,
			const double kTime) const{
    uint64_t B_space = BSpace();
    uint64_t L1space = L1Space();
    uint64_t L2L3space = L2L3Space();
    uint64_t L_space = LSpace();
    uint64_t H_space = HashSpace();
    uint64_t other_space = OtherSpace();
    uint64_t dict_space = B_space + L_space;
    uint64_t P_space = sposlp_.cpd.ByteSize();
    uint64_t W_space = dict_space + H_space + other_space + P_space;

    cout //<< kCnt << "\t"
	 << NumRules() << "\t"
	 << DictNumRules() << "\t"
	 << B_space << "\t"
	 << L1space << "\t"
	 << L2L3space << "\t"
	 << dict_space << "\t"
	 << H_space << "\t"
	 << other_space << "\t"
	 << W_space << "\t"
    << (double)sposlp_.cpd.VSize() / 1048576 << "\t"
    << (double)sposlp_.cpd.LSize() / 1048576  << "\t"
    << (double)sposlp_.cpd.PqSize() / 1048576  << "\t"
    //<< (double)sposlp_.cpd.FSize() / 1048576  << "\t"
    << (double)sposlp_.cpd.ByteSize() / 1048576  << "\t"
    << (double)(dict_space + H_space + other_space) / 1048576 << "\t"
	 //<< static_cast<double>(dict_space)/static_cast<double>(kCnt)*100 << "\t"
	 //<< static_cast<double>(W_space)/static_cast<double>(kCnt)*100 << "\t"
	 << kTime << "\t"
	 << endl;

  }

  double SOLCA::GetTimeOfDaySec() const{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
  }

  uint64_t SOLCA::Compress(const string& kInputFileName,
			   const string& kOutputFileName,
			   const bool    kEraseBr,
         const int kSize,
         const string& kPosFileName){
    ifstream ifs(kInputFileName.c_str());
    double start = GetTimeOfDaySec();
    Init(kOutputFileName);
    sposlp_.cpd.Init(kSize);
    uint64_t cnt = 0;
    for (char c; ifs && ifs.get(c);) {
      if (kEraseBr) {
        if (c == '\n') continue;
      }
      if (cnt % 10000000ULL == 0 && cnt != 0) {
        //PrintLogs(cnt, GetTimeOfDaySec() - start);
      }
      Node n(c,cnt);
      sposlp_.PushToBuffer(n);
      BuildPOSLP(n, 0);
      ++cnt;
    }

    //PrintLogs(cnt, GetTimeOfDaySec() - start);
    ProcessLastSymbols(cnt,
		       start);
    double times = GetTimeOfDaySec() - start;
    //PrintLogs(cnt, times);
    cout << "Working Space:" << (double)(BSpace()+LSpace()+HashSpace()+OtherSpace()+sposlp_.cpd.ByteSize())/1048576 << "(MB) Time:"<< times <<"(sec)"<<endl;
    sposlp_.cpd.PrintDescend(kPosFileName);
    sposlp_.Clear();
    return 1;
  }


  void SOLCA::RepeatDecompress(const uint64_t kVar,
			       ofstream &ofs){
    if(kVar < kAlphabetSize){
      ofs << (char)kVar;
    }
    else{
      RepeatDecompress(sposlp_.Left(kVar),
			   ofs);
      RepeatDecompress(sposlp_.Right(kVar),
		       ofs);
    }
  }
  
  uint64_t SOLCA::Decompress(const string& kInputFileName,
			     const string& kOutputFileName){
    ifstream ifs(kInputFileName.c_str());
    ofstream ofs(kOutputFileName.c_str());
    
    sposlp_.Load(ifs);
    ifs.close();
  
    RepeatDecompress(sposlp_.NumRules() - 1,
		     ofs);
    ofs.close();
    return 1;
  }

  
} //namespace comp
