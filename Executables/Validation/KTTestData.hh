/*
 * KTTestData.hh
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#ifndef NYMPH_KTTESTDATA_HH_
#define NYMPH_KTTESTDATA_HH_

#include "KTCoreData.hh"
#include "KTMemberVariable.hh"


namespace Nymph
{

    class KTTestData : public KTData
    {
        public:
            KTTestData();
            virtual ~KTTestData();

            MEMBERVARIABLE(bool, IsAwesome);

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version );
    };

    DEFINE_EXT_DATA( KTTestData, "test" ); // defines KTTestDataExt


    class KTTestBaseData : public KTData
    {
        public:
            KTTestBaseData() :
                KTData(),
                fFunniness(1000.)
            {}
            virtual ~KTTestBaseData() {}

            MEMBERVARIABLE(double, Funniness);

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version );
    };

    DEFINE_EXT_DATA_2( KTTestDerived1DataExt, KTTestBaseData, "test-derived-1" );
    DEFINE_EXT_DATA_2( KTTestDerived2DataExt, KTTestBaseData, "test-derived-2" );


    template< class Archive >
    void KTTestData::serialize( Archive& ar, const unsigned version )
    {
        std::cout << "### serialize for KTTestData" << std::endl;
        ar & bs::base_object< KTData >( *this );
        ar & fIsAwesome;
        return;
    }

    template< class Archive >
    void KTTestBaseData::serialize( Archive& ar, const unsigned version )
    {
        std::cout << "### serialize for KTTestBaseData" << std::endl;
        ar & bs::base_object< KTData >( *this );
        ar & fFunniness;
        return;
    }


} /* namespace Nymph */

//BOOST_CLASS_EXPORT_KEY( Nymph::KTTestBaseData );
//BOOST_CLASS_EXPORT_KEY( Nymph::KTTestDerived1DataExt );
//BOOST_CLASS_EXPORT_KEY( Nymph::KTTestDerived2DataExt );
//BOOST_CLASS_EXPORT_KEY( Nymph::KTTestData );
//BOOST_CLASS_EXPORT_KEY( Nymph::KTTestDataExt );

#endif /* NYMPH_KTTESTDATA_HH_ */
