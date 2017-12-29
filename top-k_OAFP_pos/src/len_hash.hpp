/* len_hash.hpp
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

#ifndef LEN_HASH_HPP_
#define LEN_HASH_HPP_

#include <iostream>
#include <cstdint>
#include <vector>
#include <stack>
#include "constant_numbers.hpp"
#include "common_functions.hpp"
#include "varlist.hpp"

class L_Hash{ //constant space variable hash table;
private:
  std::vector<VarList*> varlist_;
  std::vector<uint64_t> length_;
  std::vector<uint32_t> next_;
  std::vector<uint32_t> hash_table_;
  std::vector<uint32_t> not_used_;

  //const uint32_t hash_size_ = kPrimes[16];
  uint32_t hash_size_;
  uint64_t num_frules_;
  uint64_t insert_ht_pos_;
  uint32_t num_not_used_;
  uint64_t num_bucket_;

public:
  L_Hash(): varlist_(0),
    length_(0),
		next_(0),
		hash_table_(0),
		not_used_(0),
    hash_size_(0),
    num_frules_(0),
    insert_ht_pos_(0),
		num_not_used_(0){};
  ~L_Hash(){};
  
  void Init(const uint32_t kHashSize);
  bool IsExist(const uint64_t kLen);
  void Register(VarList* kVlist, const uint64_t kLen);//return pointer of vector<Vardata*>
  void Remove(const uint64_t kLen);
  void VarPush(VarData* kVdata, const uint64_t kLen);

  uint64_t ByteSize() const;
private:
  void InsertToHash(const uint64_t kInsertPos);
  void DeleteFromHash(const uint64_t kPos);
  void PrintCRDict();

};

#endif // LEN_HASH_HPP_
