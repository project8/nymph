/*
 * KTTestData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#include "KTTestData.hh"

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

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
/*
    template<>
    void KTTestData::serialize< boost::archive::polymorphic_iarchive >( boost::archive::polymorphic_iarchive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fIsAwesome;
        return;
    }

    template<>
    void KTTestData::serialize< boost::archive::polymorphic_oarchive >( boost::archive::polymorphic_oarchive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fIsAwesome;
        return;
    }

    template<>
    void KTTestBaseData::serialize< boost::archive::polymorphic_iarchive >( boost::archive::polymorphic_iarchive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fFunniness;
        return;
    }

    template<>
    void KTTestBaseData::serialize< boost::archive::polymorphic_oarchive >( boost::archive::polymorphic_oarchive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fFunniness;
        return;
    }
*/
} /* namespace Nymph */

