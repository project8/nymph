/*
 * KTTestData.cc
 *
 *  Created on: Jun 1, 2017
 *      Author: obla999
 */

#include "KTTestData.hh"

namespace Nymph
{
    KTTestData::KTTestData() :
        KTData(),
        fIsAwesome( false )
    {
        std::cout << "### KTTestData constructor" << std::endl;
    }

    KTTestData::~KTTestData()
    {
        std::cout << "### KTTestData destructor" << std::endl;
    }

    KTTestBaseData::KTTestBaseData() :
        KTData(),
        fFunniness(1000.)
    {
        std::cout << "### KTTestBaseData constructor" << std::endl;
    }

    KTTestBaseData::~KTTestBaseData()
    {
        std::cout << "### KTTestBaseData destructor" << std::endl;
    }


} /* namespace Nymph */

