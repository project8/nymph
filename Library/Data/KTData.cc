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

    KTDataRider::~KTDataRider()
    {}

    template<>
    bool KTExtensibleStructCore< KTDataRider >::fDoExtensibleSerialize = false;

} /* namespace Nymph */
