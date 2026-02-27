/*
 * TestDataClasses.hh
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_TESTING_TESTDATACLASSES
#define NYMPH_TESTING_TESTDATACLASSES

#include "Data.hh"
#include "MemberVariable.hh"

namespace NymphTesting
{
    class TestData1 : public Nymph::Data
    {
        public:
            TestData1() :
                    Data(),
                    fIValue1( 0 ),
                    fIValue2( 5 )
            {}

            virtual ~TestData1()
            {}

            MEMVAR( int, IValue1 );
            MEMVAR( int, IValue2 );
    };

    class TestData2 : public Nymph::Data
    {
        public:
            TestData2() :
                    Data(),
                    fDValue1( 0. ),
                    fDValue2( 10. )
            {}

            virtual ~TestData2()
            {}

            MEMVAR( double, DValue1 );
            MEMVAR( double, DValue2 );
    };


} /* namespace Nymph */

#endif /* NYMPH_TESTING_TESTDATACLASSES */
