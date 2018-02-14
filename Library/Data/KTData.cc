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
    {
        std::cout << "### KTDataRider for <" << fName << "> constructor" << std::endl;
    }

    KTDataRider::KTDataRider( const KTDataRider& orig ) :
            fName( orig.fName )
    {}

    KTDataRider::~KTDataRider()
    {
        std::cout << "### KTDataRider for <" << fName << "> destructor" << std::endl;
    }

} /* namespace Nymph */
