#include "varlist.hpp"

using namespace std;


void VarList::Init(){
	vector<VarData*>().swap(varlist_);
}

void VarList::Set(VarData* kVdata){
	Init();
	varlist_.push_back(kVdata);
}

void VarList::Push(VarData* kVdata){
	varlist_.push_back(kVdata);
}

uint64_t VarList::Pop(){
	uint64_t var = varlist_[varlist_.size()-1]->GetVar();
	//free(varlist_[varlist_.size()-1]);
	varlist_.pop_back();
	return var;
}

bool VarList::IsEmpty(){
	return varlist_.empty();
}

void VarList::Free(){
	vector<VarData*>().swap(varlist_);
}

VarData* VarList::VpPop(){
	VarData* vp = varlist_[varlist_.size()-1];
	varlist_.pop_back();
	return vp;
}

uint64_t VarList::Size(){
	return varlist_.size();
}

uint64_t VarList::GetVar(){
	return varlist_[varlist_.size()-1]->GetVar();
}

uint64_t VarList::GetFreq(){
	return varlist_[varlist_.size()-1]->GetFreq();
}