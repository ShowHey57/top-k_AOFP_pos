#include "pri_q.hpp"

using namespace std;


bool Pri_Q::IsLonger(const uint64_t kLength){
	if(priq_.top().length_ < kLength){
		return true;
	}else{
		return false;
	}
}

bool Pri_Q::IsTopEmpty(){
	return priq_.top().varlist_->IsEmpty();
}

uint64_t Pri_Q::VarPop(){//L_Hsh to douki ga hituyou
	return priq_.top().varlist_->Pop();
}


uint64_t Pri_Q::GetLen(){
	return priq_.top().length_;
}

void Pri_Q::Pop(){
	priq_.pop();
}

void Pri_Q::Push(VarList* kVlist, const uint64_t kLength){
	priq_.push(Cont(kVlist, kLength));
	//cout << "v:" << priq_.top().varlist_->GetVar() << " f:" << priq_.top().varlist_->GetFreq() <<endl;
}

bool Pri_Q::IsEmpty(){
	return priq_.empty();
}

VarData* Pri_Q::VpPop(){
	return priq_.top().varlist_->VpPop();
}

VarList* Pri_Q::TopVlist(){
	return priq_.top().varlist_;
}

uint64_t Pri_Q::ByteSize()const{
	return sizeof(Pri_Q) + priq_.size()*sizeof(Cont); 
}