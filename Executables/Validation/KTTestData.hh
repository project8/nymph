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
    };

    DEFINE_EXT_DATA( KTTestData ); // defines KTTestDataExt


    class KTTestBaseData : public KTData
    {
        public:
            KTTestBaseData() :
                KTData( "poly-data" ),
                fFunniness(1000.)
            {}
            virtual ~KTTestBaseData() {}

            MEMBERVARIABLE(double, Funniness);
    };

    DEFINE_EXT_DATA_2( KTTestDerived1DataHandle, KTTestBaseData );
    DEFINE_EXT_DATA_2( KTTestDerived2DataHandle, KTTestBaseData );

} /* namespace Nymph */

#endif /* NYMPH_KTTESTDATA_HH_ */
