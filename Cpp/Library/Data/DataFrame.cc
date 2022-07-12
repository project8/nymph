/*
 *  DataFrame.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */

#include "DataFrame.hh"

namespace Nymph
{
    DataFrame::DataFrame() :
        fDataObjects()
    {
	std::unordered_map< int, double > emptyMap;	
	fDataObjectsTest = emptyMap;
    }

    DataFrame::~DataFrame()
    {}

} /* namespace Nymph */
