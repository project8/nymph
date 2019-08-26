/*
 * TestExt.cc
 *
 *  Created on: Aug 23, 2019
 *      Author: N.S. Oblath
 */

#include "KTExtensible.hh"

#include "catch.hpp"

namespace Nymph
{
    class MyBaseClass
    {
        public:
            MyBaseClass() {}
            virtual ~MyBaseClass() {}
    };

    class MyClass1 : public KTExtensible< MyClass1, MyBaseClass >
    {
        public:
            MyClass1() : KTExtensible< MyClass1, MyBaseClass >(), fValue() {}
            MyClass1( const MyClass1& orig ) : KTExtensible< MyClass1, MyBaseClass >( orig ), fValue( orig.fValue ) {}
            virtual ~MyClass1() {};
        public:
            int fValue;
    };

    class MyClass2 : public KTExtensible< MyClass2, MyBaseClass >
    {
        public:
            MyClass2() : KTExtensible< MyClass2, MyBaseClass >(), fValue() {}
            MyClass2( const MyClass2& orig ) : KTExtensible< MyClass2, MyBaseClass >( orig ), fValue( orig.fValue ) {}
            virtual ~MyClass2() {};
        public:
            int fValue;
    };

}

TEST_CASE( "extensible", "[utility]" )
{
    using namespace Nymph;

    std::shared_ptr< MyClass1 > data1 = std::make_shared< MyClass1 >();
    data1->fValue = 5;

    MyClass2& data2 = data1->Of< MyClass2 >();
    data2.fValue = 3;

    std::shared_ptr< MyClass2 > data2Shared = data1->Share< MyClass2 >();

    SECTION( "Basic extensibility and values" )
    {
        REQUIRE( data1->Has< MyClass1 >() );
        REQUIRE( data1->Has< MyClass2 >() );
        REQUIRE_FALSE( data2.Has< MyClass1 >() );
        REQUIRE( data2.Has< MyClass2 >() );

        REQUIRE( data1->fValue == 5 );
        REQUIRE( data2.fValue == 3 );

        REQUIRE( data2Shared->fValue == 3 );

        REQUIRE( data1->size() == 2 );
        REQUIRE( data2.size() == 1 );
    }

    std::shared_ptr< MyClass1 > copyOfData1 = std::make_shared< MyClass1 >( *data1 );
    MyClass2& copyOfData2 = copyOfData1->Of< MyClass2 >();

    SECTION( "Copying data" )
    {
        REQUIRE( copyOfData1->fValue == data1->fValue );
        REQUIRE_FALSE( copyOfData1 == data1 );
        REQUIRE( copyOfData2.fValue == data2.fValue );
    }

    std::shared_ptr< MyClass2 > detatchedCopyOfData2 = copyOfData1->Detatch< MyClass2 >();

    SECTION( "Detatching" )
    {
        REQUIRE( copyOfData1->size() == 1 );
        REQUIRE( detatchedCopyOfData2->size() == 1 );
    }
}
