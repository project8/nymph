/*
 * TestData.cc
 *
 *  Created on: Feb 10, 2018
 *      Author: obla999
 */

#include "TestData.hh"
#include "CoreData.hh"

#include "Logger.hh"

using namespace Nymph;

LOGGER(testlog, "TestData")

int main()
{

    ExtCoreData data;
    TestDataExt testData("test");
    LINFO(testlog, data.Name());
    LINFO(testlog, testData.Name());

    return 0;
}
