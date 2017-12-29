#ifndef PRI_Q_HPP_
#define PRI_Q_HPP_

#include <cstdint>
#include <vector>
#include <iostream>
#include <queue>
#include "constant_numbers.hpp"
#include "common_functions.hpp"
#include "varlist.hpp"
#include "cntnr.hpp"

using namespace std;

class Pri_Q
{
private:
	priority_queue<Cont, vector<Cont>, greater<Cont>> priq_;

public:
	Pri_Q(){};
	~Pri_Q(){};


	bool IsLonger(const uint64_t kLength);
	bool IsTopEmpty();
	uint64_t GetLen();
	uint64_t VarPop();
	void Pop();
	void Push(VarList* kVlist, const uint64_t kLength);

	bool IsEmpty();
	VarData* VpPop();
	VarList* TopVlist();

	uint64_t ByteSize()const;

};
















#endif // PRI_Q_HPP_