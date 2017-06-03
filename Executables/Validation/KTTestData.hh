/*
 * KTTestData.hh
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#ifndef NYMPH_KTTESTDATA_HH_
#define NYMPH_KTTESTDATA_HH_

#include "KTData.hh"
#include "KTMemberVariable.hh"


namespace Nymph
{

    class KTTestData : public KTExtensibleData< KTTestData >
    {
        public:
            KTTestData();
            virtual ~KTTestData();

            MEMBERVARIABLE(bool, IsAwesome);

        public:
            static const std::string sName;

    };


    template< class XDerivedDataType >
    class KTTestPolyDataBase : public KTExtensibleData< XDerivedDataType >
    {
        public:
            KTTestPolyDataBase() :
                KTExtensibleData< XDerivedDataType >(),
                fFunniness(1000.)
            {}
            virtual ~KTTestPolyDataBase() {}

            MEMBERVARIABLE(double, Funniness);
    };

    class KTTestDerived1Data : public KTTestPolyDataBase< KTTestDerived1Data >
    {
        public:
            KTTestDerived1Data();
            virtual ~KTTestDerived1Data();

        public:
            static const std::string sName;
    };

    class KTTestDerived2Data : public KTTestPolyDataBase< KTTestDerived2Data >
    {
        public:
            KTTestDerived2Data();
            virtual ~KTTestDerived2Data();

        public:
            static const std::string sName;
    };

} /* namespace Nymph */

#endif /* NYMPH_KTTESTDATA_HH_ */
