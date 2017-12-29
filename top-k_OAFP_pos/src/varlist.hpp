#ifndef VAR_LIST_HPP_
#define VAR_LIST_HPP_

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "constant_numbers.hpp"
#include "common_functions.hpp"
#include "vardata.hpp"

class VarList{
private:
	std::vector<VarData*> varlist_; //pointer of VarData's vector

public:
	VarList(): varlist_(0){};
	~VarList(){};

	void Init();
	void Set(VarData* kVdata);
	void Push(VarData* kVdata);
	uint64_t Pop();
	bool IsEmpty();
	void Free();

	VarData* VpPop();
	uint64_t Size();
	uint64_t GetVar();
	uint64_t GetFreq();
};

#endif // VAR_LIST_HPP_