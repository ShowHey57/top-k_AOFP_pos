#ifndef VAR_DATA_HPP_
#define VAR_DATA_HPP_

#include <cstdint>
#include <vector>
#include "constant_numbers.hpp"
#include "common_functions.hpp"


class VarData{
private:
	uint64_t var_;    //変数コード
	uint64_t freq_;   //頻度
	std::vector<uint64_t> pos_;

public:
	VarData(): var_(0), freq_(0),pos_(){};
	VarData(const uint64_t kVar): var_(kVar), freq_(2){};
	~VarData(){};
	
	void Set(const uint64_t kVar, const uint64_t kPos);
	void Increment(const uint64_t kPos);
	uint64_t GetVar(){return var_;};
	uint64_t GetFreq(){return freq_;};
	uint64_t PosSize(){return pos_.size();};
	uint64_t GetPos(const uint64_t kPos){return pos_[kPos];};
};


#endif // VAR_DATA_HPP_