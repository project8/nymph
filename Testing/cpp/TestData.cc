/*
 * TestData.cc
 *
 *  Created on: Feb 10, 2018
 *      Author: obla999
 */

#include "KTTestData.hh"

#include "KTLogger.hh"

using namespace Nymph;

LOGGER(testlog, "TestData")

int main()
{

    KTCoreDataExt data;
    KTTestDataExt& testData = data.Of< KTTestDataExt >();
    KTINFO(testlog, data.Name());
    KTINFO(testlog, testData.Name());

    return 0;
}
