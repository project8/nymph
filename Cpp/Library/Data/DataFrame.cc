/*
 *  DataFrame.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */

#include "DataFrame.hh"

namespace Nymph
{
    IndexableData::IndexableData( std::unique_ptr< Data >&& dataPtr, std::type_index&& typeIndex, std::string&& typeName ) :
            fDataPtr( std::move(dataPtr) ),
            fTypeIndex( std::move(typeIndex) ),
            fTypeName( std::move(typeName) )
    {}


    DataFrame::DataFrame() :
        fDataObjects(),
        fDataMapByType( fDataObjects.get<type_t>() ),
        fDataMapByTypeName( fDataObjects.get<tname_t>() )
    {}

    DataFrame::~DataFrame()
    {}

} /* namespace Nymph */
