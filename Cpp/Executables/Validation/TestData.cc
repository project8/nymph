/*
 * TestData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#include "TestData.hh"

namespace Nymph
{
    TestDataExt::TestDataExt() :
        ExtCoreData(),
        fIsAwesome( false )
    {
        std::cout << "### TestData constructor" << std::endl;
    }

    TestDataExt::TestDataExt(std::string name) :
	ExtCoreData( name )
    {
    }

    TestDataExt::~TestDataExt()
    {
        std::cout << "### TestData destructor" << std::endl;
    }

    TestBaseData::TestBaseData() :
        Data(),
        fFunniness(1000.)
    {
        std::cout << "### TestBaseData constructor" << std::endl;
    }

    TestBaseData::~TestBaseData()
    {
        std::cout << "### TestBaseData destructor" << std::endl;
    }

/*
    class TestDataExt : public TestData, public ExtCoreData
    {  
	TestData(),
	ExtCoreData() 
        public:
            TestDataExt();
            TestDataExt(std::string name);
            virtual ~TestDataExt();
    }   
*/
/*
    TestDataExt::TestDataExt() :
        TestData(),
        ExtCoreData()
    {
    }

    TestDataExt::TestDataExt(std::string name) :
        TestData(),
        ExtCoreData( name )
    {
    }

    TestDataExt::~TestDataExt() 
    {
    }
*/
} /* namespace Nymph */

