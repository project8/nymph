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
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
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
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
    };

    DEFINE_EXT_DATA_2( KTTestDerived1DataExt, KTTestBaseData, "test-derived-1" );
    DEFINE_EXT_DATA_2( KTTestDerived2DataExt, KTTestBaseData, "test-derived-2" );


    template< class Archive >
    void KTTestData::serialize( Archive& ar )
    {
            std::cout << "### serialize for KTTestData" << std::endl;
            ar( cereal::base_class< KTData >( this ), CEREAL_NVP(fIsAwesome) );
    }

    template< class Archive >
    void KTTestBaseData::serialize( Archive& ar )
    {
            std::cout << "### serialize for KTTestBaseData" << std::endl;
            ar( cereal::base_class< KTData >( this ), CEREAL_NVP(fFunniness) );
    }

} /* namespace Nymph */

#endif /* NYMPH_KTTESTDATA_HH_ */
