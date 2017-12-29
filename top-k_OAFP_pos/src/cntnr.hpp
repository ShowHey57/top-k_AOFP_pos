#ifndef CNTNR_HPP_
#define CNTNR_HPP_

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "constant_numbers.hpp"
#include "common_functions.hpp"
#include "vardata.hpp"
#include "varlist.hpp"
#include "boost/shared_ptr.hpp"

class Cont{

public:
	VarList* varlist_; //pointer of VarData's vector
	uint64_t length_;

	Cont(): varlist_(0), length_(0){};
	Cont(VarList* vlist, uint64_t kLen):varlist_(vlist), length_(kLen){};

	bool operator > (const Cont &cont) const {
		return (length_ >  cont.length_);
	}


};

#endif // CNTNR_HPP_