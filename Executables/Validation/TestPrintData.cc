/*
 * TestPrintData.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "KTCut.hh"
#include "KTLogger.hh"
#include "KTPrintDataStructure.hh"

#include "KTTestCuts.hh"
#include "KTTestData.hh"

#include "param.hh"

KTLOGGER(testlog, "TestPrintData");


using namespace Nymph;
using namespace std;

int main()
{
    KTDataHandle dataHandle = CreateNewDataHandle();
    KTCoreDataExt& data = dataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& testData = dataHandle->Of< KTTestDataExt >();

    KTINFO(testlog, "Applying awesome cut");
    KTAwesomeCut cut;
    cut.Apply(data, testData);

    KTINFO(testlog, "Applying not-awesome cut");
    KTNotAwesomeCut naCut;
    naCut.Apply(data, testData);

    KTPrintDataStructure printer;

    KTINFO(testlog, "Printing data structure");
    printer.PrintDataStructure(dataHandle);

    KTINFO(testlog, "Printing cut structure");
    printer.PrintCutStructure(dataHandle);

    return 0;
}
