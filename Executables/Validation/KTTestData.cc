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
        KTExtensibleData< KTTestData >(),
        fIsAwesome(false)
    {}

    KTTestData::~KTTestData()
    {}

    const std::string KTTestData::sName = "test-data";


    KTTestDerived1Data::KTTestDerived1Data() :
            KTTestPolyDataBase< KTTestDerived1Data >()
    {}

    KTTestDerived1Data::~KTTestDerived1Data()
    {}

    const std::string KTTestDerived1Data::sName = "test-derived-1-data";

    KTTestDerived2Data::KTTestDerived2Data() :
            KTTestPolyDataBase< KTTestDerived2Data >()
    {}

    KTTestDerived2Data::~KTTestDerived2Data()
    {}

    const std::string KTTestDerived2Data::sName = "test-derived-2-data";

} /* namespace Nymph */

