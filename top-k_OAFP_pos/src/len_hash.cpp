/* len_hash.cpp
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

#include "len_hash.hpp"

using namespace std;

void L_Hash::Init(const uint32_t kHashSize){

  hash_size_ = kHashSize;
  num_frules_ = 0;
  insert_ht_pos_ = 0;
  num_not_used_ = hash_size_;
  num_bucket_ = 1;
  
  CFunc::ResizeVec(varlist_, hash_size_);
  CFunc::ResizeVec(length_, hash_size_);
  CFunc::ResizeVec(hash_table_, hash_size_);
  CFunc::ResizeVec(next_, hash_size_);
  CFunc::ResizeVec(not_used_, hash_size_);

  for(size_t i = 0; i < hash_size_; i++){
    varlist_[i] = 0;
    length_[i] = 0;
    hash_table_[i] = hash_size_;
    next_[i] = hash_size_;
    not_used_[i] = hash_size_ - i - 1;
  }

}

bool L_Hash::IsExist(const uint64_t kLen){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kLen);
  uint64_t pos = hash_table_[insert_ht_pos_];
  while(pos != hash_size_){
    if(length_[pos] == kLen){
      return true;
    }
    pos = next_[pos];
  }

  return false;
}

void L_Hash::Register(VarList* kVlist, const uint64_t kLen){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kLen);
  uint32_t insert_pos = not_used_[--num_not_used_];
  InsertToHash(insert_pos);

  varlist_[insert_pos] = kVlist;
  length_[insert_pos] = kLen;
  num_frules_++;
}


void L_Hash::VarPush(VarData* kVdata, const uint64_t kLen){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kLen);
  uint64_t pos = hash_table_[insert_ht_pos_];
  while(pos != hash_size_){
    if(length_[pos] == kLen){
      varlist_[pos]->Push(kVdata);
      //cout << "v:"<< varlist_[pos]->GetVar() << " f:" << varlist_[pos]->GetFreq() << endl;
      return;
    }
    pos = next_[pos];
  }
}

void L_Hash::Remove(const uint64_t kLen){
  insert_ht_pos_ = CFunc::ComputeHashVal(hash_size_,kLen);
  uint64_t pos = hash_table_[insert_ht_pos_];
  while(pos != hash_size_){
    if(length_[pos] == kLen){
      DeleteFromHash(pos);
      not_used_[num_not_used_++] = pos;
      num_frules_--;
    }
    pos = next_[pos];
  }
}

void L_Hash::PrintCRDict(){
  
  for(size_t i = 0; i < hash_size_; i++){
    //std::cout << i << " " << var_[i] << " " << left_[i] << " " << right_[i] << " " << next_[i] << endl;
  }
  
  cout << "hash" << endl;
  for(size_t i = 0; i < hash_size_; i++){
    cout << i << " " << hash_table_[i] << endl;
  }
}

void L_Hash::InsertToHash(const uint64_t kInsertPos){
  uint64_t tmp = hash_table_[insert_ht_pos_];
  hash_table_[insert_ht_pos_] = kInsertPos;
  next_[kInsertPos] = tmp;
}

void L_Hash::DeleteFromHash(const uint64_t kPos){
  
  uint64_t hash_pos = CFunc::ComputeHashVal(hash_size_, length_[kPos]);

  uint64_t pos = hash_table_[hash_pos];
  if(pos == kPos){
    hash_table_[hash_pos] = next_[pos];
    next_[pos] = hash_size_;
    delete(varlist_[pos]);
    varlist_[pos] = 0;
    length_[pos] = 0;
  }
  else{
    uint64_t prev_pos = pos;
    pos = next_[pos];
    while(pos != hash_size_){
      if(pos == kPos){
        next_[prev_pos] = next_[pos];
        next_[pos] = hash_size_;
        free(varlist_[pos]);
        varlist_[pos] = 0;
        length_[pos] = 0;
        break;
      }
      prev_pos = pos;
      pos = next_[pos];
    }
  }
}

uint64_t L_Hash::ByteSize() const{
  return sizeof(L_Hash) +
    hash_size_*sizeof(VarList*) +
    hash_size_*sizeof(VarList) +
    hash_size_*sizeof(uint64_t) +
    3*hash_size_*sizeof(uint32_t);
}
