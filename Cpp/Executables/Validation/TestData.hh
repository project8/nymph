/*
 * TestData.hh
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#ifndef NYMPH_TESTDATA_HH_
#define NYMPH_TESTDATA_HH_

#include "Data.hh"
#include "CoreData.hh"
#include "MemberVariable.hh"
#include "param.hh"

namespace Nymph
{

    class TestDataExt : public ExtCoreData
    {
        public:
            TestDataExt();
            TestDataExt(std::string name);
            virtual ~TestDataExt();

            MEMVAR(bool, IsAwesome);
            MEMVAR_REF( CutStatus, GetCutStatus );
        private:
//--------RemovedSerial--------------
/*
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
*/
    };

//    DEFINE_EXT_DATA( TestData, "test" ); // defines KTTestDataExt

/*
    template< class XDerivedType >
    class ExtensibleTestData : public TestData, public Extensible< XDerivedType, TestData >
    {
        ExtensibleTestData(): TestData(), 
        virtual ~ExtensibleTestData()

        MEMVAR_REF( "test", Name );
*/

    class TestBaseData : public Data
    {
        public:
            TestBaseData();
            virtual ~TestBaseData();

            MEMVAR(double, Funniness);

        private:
//--------RemovedSerial--------------
/*
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
*/
    };

//    DEFINE_EXT_DATA_2( KTTestDerived1DataExt, KTTestBaseData, "test-derived-1" );
//    DEFINE_EXT_DATA_2( KTTestDerived2DataExt, KTTestBaseData, "test-derived-2" );

//--------RemovedSerial--------------
/*
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
*/
} /* namespace Nymph */

#endif /* NYMPH_KTTESTDATA_HH_ */
