/*
 * KTData.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "KTData.hh"

namespace Nymph
{
    KTData::KTData( const std::string& name ) :
            fName( name )
    {}

    KTData::KTData( const KTData& orig ) :
            fName( orig.fName )
    {}

    KTData::KTData( KTData&& orig ) :
            fName( std::move( orig.fName ) )
    {}

    KTData::~KTData()
    {}

} /* namespace Nymph */
