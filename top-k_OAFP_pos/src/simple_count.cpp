/* freq_crdict.cpp
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

#include "simple_count.hpp"

using namespace std;

void SimpleCount::Init(const uint32_t SimpleSize){

  hash_size_ = SimpleSize;
  num_frules_ = 0;
  insert_ht_pos_ = 0;
  num_not_used_ = hash_size_;
  num_bucket_ = 1;
  cnt_ = 0;
  vector<bool>().swap(f_vec);
  for(int i =0; i < 256; ++i){
    f_vec.push_back(0);
  }
  //latest_var = 0;
  
  CFunc::ResizeVec(var_, hash_size_);
  CFunc::ResizeVec(length_, hash_size_);
  CFunc::ResizeVec(freq_, hash_size_);
  CFunc::ResizeVec(hash_table_, hash_size_);
  CFunc::ResizeVec(next_, hash_size_);
  CFunc::ResizeVec(not_used_, hash_size_);
  //CFunc::ResizeVec(test_timing, hash_size_);

  for(size_t i = 0; i < hash_size_; i++){
    var_[i] = kDummyCode;
    length_[i] = 0;
    freq_[i] = 0;
    hash_table_[i] = hash_size_;
    next_[i] = hash_size_;
    not_used_[i] = hash_size_ - i - 1;
  }

}

void SimpleCount::PrintSimple(){
  uint64_t max_len = 0;
  uint64_t max_freq = 0;
  uint64_t len_var,freq_var;
  uint64_t num_ptn =0;
  //uint64_t latest_timing = 0;
  ofstream outputfileS("Simple_Out.txt");
  outputfileS.close();
  for(size_t i = 0; i < hash_size_; i++){
    if(freq_[i] >= 2){
      ofstream outputfileS("Simple_Out.txt",ios::app);
      outputfileS << "[" << setw(8) << right << i << "] code:" << std::setw(8) << right << var_[i]
                  << " freq:"<< setw(8) << right << freq_[i]
                  << " len:"<< setw(8) << right << length_[i] <<endl;
      if(max_len < length_[i]){
        max_len = length_[i];
        len_var = var_[i];
      }
      if(max_freq < freq_[i]){
        max_freq = freq_[i];
        freq_var = var_[i];
      }
      /*if(latest_var > test_timing[i]){
        cout << "error exits"<<endl;
      }else{
        latest_timing = test_timing[i];
      }*/
      ++num_ptn;
    }
  }
  cout << "max_freq:" << max_freq << " (var = "<< freq_var <<")    max_len:" << max_len << "(var = " << len_var << ")     num_ptn:"<< num_ptn << endl;
}


void SimpleCount::Count(const uint64_t kVar,
			 const uint64_t kLength){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kVar);
  uint64_t pos = hash_table_[insert_ht_pos_];

  //if(pos != hash_size_){
    while(pos != hash_size_){
      if(var_[pos] == kVar){
        freq_[pos]++;
        return;
      }
      pos = next_[pos];
    }
  //}else{
    uint32_t insert_pos = not_used_[--num_not_used_];
    InsertToHash(insert_pos);

    var_[insert_pos] = kVar;
    length_[insert_pos] = kLength;
    freq_[insert_pos] = 1;
    num_frules_++;
    if(num_frules_ == hash_size_){
      DeleteNotFreqRules();
      num_bucket_++;
      ++cnt_;
    }
  //}
}

void SimpleCount::InsertToHash(const uint64_t kInsertPos){
  uint64_t tmp = hash_table_[insert_ht_pos_];
  hash_table_[insert_ht_pos_] = kInsertPos;
  next_[kInsertPos] = tmp;
}

void SimpleCount::DeleteNotFreqRules(){
  while(num_frules_ == hash_size_){
    for(size_t i = 0; i < hash_size_; i++){
      if(--freq_[i] == 0){
	DeleteFromHash(i);
	not_used_[num_not_used_++] = i;
	num_frules_--;
      }
    }
  }
}

void SimpleCount::DeleteFromHash(const uint64_t kPos){
  
  uint64_t hash_pos = CFunc::ComputeHashVal(hash_size_,
					    var_[kPos]);

  uint64_t pos = hash_table_[hash_pos];
  if(pos == kPos){
    hash_table_[hash_pos] = next_[pos];
    next_[pos] = hash_size_;
    var_[pos] = kDummyCode;
    length_[pos] = 0;
  }
  else{
    uint64_t prev_pos = pos;
    pos = next_[pos];
    while(pos != hash_size_){
      if(pos == kPos){
	next_[prev_pos] = next_[pos];
	next_[pos] = hash_size_;
	var_[pos] = kDummyCode;
	length_[pos] = 0;
	break;
      }
      prev_pos = pos;
      pos = next_[pos];
    }
  }
}

void SimpleCount::AddVar(){
	f_vec.push_back(0);
}

void SimpleCount::FreqVar(const uint64_t kVar){
	f_vec[kVar] = 1;
}

void SimpleCount::PrintCnt(){
  cout << "number of -1:" << cnt_ <<endl;
}

uint64_t SimpleCount::ByteSize() const{
  return sizeof(SimpleCount) + 
    3*hash_size_*sizeof(uint64_t) +
    3*hash_size_*sizeof(uint32_t) +
    f_vec.size()*sizeof(bool);
}
