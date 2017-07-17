/*
 * KTTestData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#include "KTTestData.hh"

//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestBaseData );
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestDerived1DataExt );
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestDerived2DataExt );
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestData );
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestDataExt );

namespace Nymph
{
    KTTestData::KTTestData() :
        KTData(),
        fIsAwesome( false )
    {}

    KTTestData::~KTTestData()
    {}

} /* namespace Nymph */

