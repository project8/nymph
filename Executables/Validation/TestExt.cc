/*
 * TestExt.cc
 *
 *  Created on: Feb 11, 2018
 *      Author: N.S. Oblath
 */

#include "KTExtTest.hh"

#include "KTLogger.hh"

#include <iostream>

KTLOGGER( testlog, "KTExtTest" );

using namespace Nymph;

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

int main()
{
    std::shared_ptr< MyClass1 > data1 = std::make_shared< MyClass1 >();
    data1->fValue = 5;

    MyClass2& data2 = data1->Of< MyClass2 >();
    data2.fValue = 3;

    KTINFO( testlog, "data1 has data1: " << data1->Has< MyClass1 >() );
    KTINFO( testlog, "data1 has data2: " << data1->Has< MyClass2 >() );
    KTINFO( testlog, "data2 has data1: " << data2.Has< MyClass1 >() );
    KTINFO( testlog, "data2 has data2: " << data2.Has< MyClass2 >() );
    KTINFO( testlog, "Value of data1: " << data1->fValue );
    KTINFO( testlog, "Value of data2: " << data2.fValue );
    KTINFO( testlog, "Size of data1: " << data1->size() );
    KTINFO( testlog, "Size of data2: " << data2.size() );

    std::shared_ptr< MyClass1 > copyOfData1 = std::make_shared< MyClass1 >( *data1 );
    MyClass2& copyOfData2 = copyOfData1->Of< MyClass2 >();

    KTINFO( testlog, "Value of data1's copy: " << copyOfData1->fValue );
    KTINFO( testlog, "Value of data2's copy: " << copyOfData2.fValue );

    std::shared_ptr< MyClass2 > detatchedCoypOfData2 = copyOfData1->Detatch< MyClass2 >();

    KTINFO( testlog, "Size of copy of data1 after detatch: " << copyOfData1->size() );
    KTINFO( testlog, "Size of detatched copy of data2: " << detatchedCoypOfData2->size() );

    return 0;
}
