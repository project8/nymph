/*
 * KTCoreData.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "KTCoreData.hh"

namespace Nymph
{
    KTCoreData::KTCoreData() :
            KTData( "data" ),
            fCounter( 0 ),
            fLastData( false ),
            fCutStatus()
    {
    }

    KTCoreData::KTCoreData( const KTCoreData& orig ) :
            KTData( orig ),
            fCounter( orig.fCounter ),
            fLastData( orig.fLastData ),
            fCutStatus( orig.fCutStatus )
    {}

    KTCoreData::KTCoreData( KTCoreData&& orig ) :
            KTData( orig ),
            fCounter( std::move( orig.fCounter ) ),
            fLastData( std::move( orig.fLastData ) ),
            fCutStatus( std::move( orig.fCutStatus ) )
    {}

    KTCoreData::~KTCoreData()
    {}

} /* namespace Nymph */
