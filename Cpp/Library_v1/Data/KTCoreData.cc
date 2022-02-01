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
            KTData(),
            fCounter( 0 ),
            fLastData( false ),
            fCutStatus()
    {
        std::cout << "### KTCoreData constructor" << std::endl;
    }

    KTCoreData::~KTCoreData()
    {
        std::cout << "### KTCoreData destrutor" << std::endl;
    }


    KTDataHandle CreateNewDataHandle()
    {
        return std::make_shared< KTCoreDataExt >();
    }


} /* namespace Nymph */
