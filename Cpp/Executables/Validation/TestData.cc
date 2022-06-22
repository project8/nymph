/*
 * TestData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#include "TestData.hh"

namespace Nymph
{
    TestData::TestData() :
        CoreData(),
        fIsAwesome( false )
    {
        std::cout << "### TestData constructor" << std::endl;
    }

    TestData::~TestData()
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
        public:
            TestDataExt();
            TestDataExt(std::string name);
            virtual ~TestDataExt();
    }   
*/

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

} /* namespace Nymph */

