/* v_hash.cpp
MIT License

Copyright (c) 2017 Yoshimasa Takabatake, Shouhei Fukunaga

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

#include "v_hash.hpp"

using namespace std;

void V_Hash::Init(const uint32_t kHashSize){

  hash_size_ = kHashSize;
  num_frules_ = 0;
  insert_ht_pos_ = 0;
  num_not_used_ = hash_size_;
  num_bucket_ = 1;
  
  CFunc::ResizeVec(var_, hash_size_);
  CFunc::ResizeVec(vardata_, hash_size_);
  CFunc::ResizeVec(hash_table_, hash_size_);
  CFunc::ResizeVec(next_, hash_size_);
  CFunc::ResizeVec(not_used_, hash_size_);

  for(size_t i = 0; i < hash_size_; i++){

    var_[i] = kDummyCode;
    vardata_[i] = 0;
    hash_table_[i] = hash_size_;
    next_[i] = hash_size_;
    not_used_[i] = hash_size_ - i - 1;
  }

}

bool V_Hash::Find(const uint64_t kVar, const uint64_t kPos){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kVar);
  uint64_t pos = hash_table_[insert_ht_pos_];
  while(pos != hash_size_){
    if(var_[pos] == kVar){
      vardata_[pos]->Increment(kPos);
      //cout << "v:"<< vardata_[pos]->GetVar() << " f:" << vardata_[pos]->GetFreq();
      return true;
    }
    pos = next_[pos];
  }

  return false;
}

bool V_Hash::IsFull(){
  if(num_frules_ == hash_size_){
    return true;
  }else{
    return false;
  }
}

void V_Hash::Register(VarData* kVdata){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kVdata->GetVar());
  uint32_t insert_pos = not_used_[--num_not_used_];
  InsertToHash(insert_pos);

  var_[insert_pos] = kVdata->GetVar();
  vardata_[insert_pos] = kVdata;
  num_frules_++;
}

void V_Hash::Remove(const uint64_t kVar){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kVar);
  uint64_t pos = hash_table_[insert_ht_pos_];
  while(pos != hash_size_){
    if(var_[pos] == kVar){
      DeleteFromHash(pos);
      not_used_[num_not_used_++] = pos;
      num_frules_--;
    }
    pos = next_[pos];
  }
}

void V_Hash::PrintCRDict(){
  
  for(size_t i = 0; i < hash_size_; i++){
    //std::cout << i << " " << var_[i] << " " << left_[i] << " " << right_[i] << " " << next_[i] << endl;
  }
  
  cout << "hash" << endl;
  for(size_t i = 0; i < hash_size_; i++){
    cout << i << " " << hash_table_[i] << endl;
  }
}

void V_Hash::InsertToHash(const uint64_t kInsertPos){
  uint64_t tmp = hash_table_[insert_ht_pos_];
  hash_table_[insert_ht_pos_] = kInsertPos;
  next_[kInsertPos] = tmp;
}

void V_Hash::DeleteFromHash(const uint64_t kPos){
  uint64_t hash_pos = CFunc::ComputeHashVal(hash_size_, var_[kPos]);

  uint64_t pos = hash_table_[hash_pos];
  if(pos == kPos){
    hash_table_[hash_pos] = next_[pos];
    next_[pos] = hash_size_;
    delete(vardata_[pos]);
    var_[pos] = kDummyCode;
    vardata_[pos] = 0;
  }
  else{
    uint64_t prev_pos = pos;
    pos = next_[pos];
    while(pos != hash_size_){
      if(pos == kPos){
        next_[prev_pos] = next_[pos];
        next_[pos] = hash_size_;
        free(vardata_[pos]);
        var_[pos] = kDummyCode;
        vardata_[pos] = 0;
        break;
      }
      prev_pos = pos;
      pos = next_[pos];
    }
  }
}

uint64_t V_Hash::ByteSize() const{
  return sizeof(V_Hash) + 
    hash_size_*sizeof(VarData*) +
    hash_size_*sizeof(VarData) +
    hash_size_*sizeof(uint64_t) +
    3*hash_size_*sizeof(uint32_t);
}