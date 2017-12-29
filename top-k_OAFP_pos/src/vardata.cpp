#include "vardata.hpp"

using namespace std;

void VarData::Increment(const uint64_t kPos){
	++freq_;
	pos_.push_back(kPos);
}

void VarData::Set(const uint64_t kVar, const uint64_t kPos){
	var_ = kVar;
	freq_ = 1;
	vector<uint64_t>().swap(pos_);
	pos_.push_back(kPos);
}