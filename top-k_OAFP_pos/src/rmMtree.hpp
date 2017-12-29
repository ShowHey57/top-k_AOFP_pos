#ifndef RMMTREE_HPP_
#define RMMTREE_HPP_

#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "constant_numbers.hpp"
#include "suc_fbtree.hpp"
#include "inner_leaf.hpp"
#include "out_leaf.hpp"

class rmMNode{
	public:
		short te; //the total excess of the area covered by v
		short min; //the minimum excess in this area
		short max; //the maximum excess in this area
		

	public:
 		rmMNode();
 		~rmMNode();
 		void Set(const short pTe, const bool pMin, const uint64_t pMax){
      		te = pTe;
      		min = pMin;
      		max = pMax;
    	}
    	void Set(const short pTe, const bool pMin){
      		te = pTe;
      		min = pMin;
    	}
 };

#endif