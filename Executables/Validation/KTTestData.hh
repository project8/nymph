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
            void Serialize( Archive& ar, const unsigned version );
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
            void Serialize( Archive& ar, const unsigned version );
    };

    DEFINE_EXT_DATA_2( KTTestDerived1DataExt, KTTestBaseData, "test-derived-1" );
    DEFINE_EXT_DATA_2( KTTestDerived2DataExt, KTTestBaseData, "test-derived-2" );


    template< class Archive >
    void KTTestData::Serialize( Archive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fIsAwesome;
        return;
    }

    template< class Archive >
    void KTTestBaseData::Serialize( Archive& ar, const unsigned version )
    {
        ar & bs::base_object< KTData >( *this );
        ar & fFunniness;
        return;
    }


} /* namespace Nymph */

#endif /* NYMPH_KTTESTDATA_HH_ */
