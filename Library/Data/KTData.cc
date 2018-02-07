/*
 * KTData.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "KTData.hh"

namespace Nymph
{
    KTData::KTData()
    {}

    KTData::~KTData()
    {}

    KTDataRider::KTDataRider() :
            fName()
    {}

    KTDataRider::KTDataRider( const KTDataRider& orig ) :
            fName( orig.fName )
    {}

    KTDataRider::~KTDataRider()
    {}

} /* namespace Nymph */
