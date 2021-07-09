/*
 * TestPrintData.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "KTLogger.hh"
#include "KTPrintDataStructure.hh"

#include "KTTestCuts.hh"

KTLOGGER(testlog, "TestPrintData");

using namespace Nymph;
using namespace std;

int main()
{
    KTDataPtr dataPtr(new KTData());
    KTData& data = dataPtr->Of< KTData >();
    KTTestData& testData = dataPtr->Of< KTTestData >();

    KTINFO(testlog, "Applying awesome cut");
    KTAwesomeCut cut;
    cut.Apply(data, testData);

    KTINFO(testlog, "Applying not-awesome cut");
    KTNotAwesomeCut naCut;
    naCut.Apply(data, testData);

    KTPrintDataStructure printer;

    KTINFO(testlog, "Printing data structure");
    printer.PrintDataStructure(dataPtr);

    KTINFO(testlog, "Printing cut structure");
    printer.PrintCutStructure(dataPtr);

    return 0;
}
