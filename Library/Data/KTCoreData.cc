/*
 * KTCoreData.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "KTCoreData.hh"

BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTCoreDataExt );

namespace Nymph
{
    KTCoreData::KTCoreData() :
            KTData(),
            fCounter( 0 ),
            fLastData( false ),
            fCutStatus()
    {
    }

    KTCoreData::~KTCoreData()
    {}


    KTDataHandle CreateNewDataHandle()
    {
        return std::make_shared< KTCoreDataExt >();
    }


} /* namespace Nymph */
