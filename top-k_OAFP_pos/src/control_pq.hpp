#ifndef CONTROL_PQ_HPP_
#define CONTROL_PQ_HPP_

#include <cstdint>
#include <vector>
#include <iostream>
#include <queue>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "vardata.hpp"
#include "varlist.hpp"
#include "v_hash.hpp"
#include "len_hash.hpp"
#include "pri_q.hpp"
#include "constant_numbers.hpp"

class Control_Pq
{
private:
	V_Hash vhash_;
	L_Hash lhash_;
	Pri_Q  priq_;
  	//std::vector<bool> f_vec; //
  	std::vector<uint64_t> fpos;

public:
	Control_Pq():vhash_(),lhash_(),priq_(){};
	~Control_Pq(){};

	void Init(const uint32_t kHashSize);
	void Count(const uint64_t kVar, const uint64_t kLen,const uint64_t kPos);

	void Print();
	void PrintDescend(const std::string& kExperimentFileName);

	uint64_t ByteSize()const;
	uint64_t VSize()const;
	uint64_t LSize()const;
	uint64_t PqSize()const;

	//uint64_t FSize()const;

	void SetPos(const uint64_t kVar, const uint64_t kPos);
	//f_vec function
	/*void AddVar();
	void FreqVar(const uint64_t kVar);
	bool IsFreq(const uint64_t kVar);
	*/
};

typedef struct Ss
{
  uint64_t var_;
  uint64_t freq_;
  uint64_t len_;
  std::vector<uint64_t> pos_;

  static bool complen(const Ss& first, const Ss& second){
    return first.len_ > second.len_;
  }

  static bool compfreq(const Ss& first, const Ss& second){
    return first.freq_ > second.freq_;
  }
}SLen;

#endif //CONTROL_PQ