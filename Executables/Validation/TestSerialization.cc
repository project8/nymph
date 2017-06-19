/*
 * TestSerialization.cc
 *
 *  Created on: Jun 12, 2017
 *      Author: obla999
 */

#include "KTLogger.hh"
#include "KTPrintDataStructure.hh"

#include "KTTestData.hh"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>

KTLOGGER(testlog, "TestPrintData");


using namespace Nymph;
using namespace std;

int main()
{
    KTDataHandle dataHandle = CreateNewDataHandle();
    KTCoreDataExt& data = dataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& testData = dataHandle->Of< KTTestDataExt >();

    // change data values from their defaults
    data.SetCounter( 5 );
    testData.SetIsAwesome( true );

    KTPrintDataStructure printer;

    KTINFO(testlog, "Printing data structure -- before saving");
    printer.PrintDataStructure(dataHandle);

    KTINFO(testlog, "KTCoreData counter: " << data.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << testData.GetIsAwesome());

    // save data to archive
    {
        // create and open a character archive for output
        std::ofstream fileOut("test_serialization_output.txt");

        boost::archive::text_oarchive oArchive(fileOut);
        // write data objects to archive
        oArchive << data << testData;
        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    KTDataHandle newDataHandle = CreateNewDataHandle();
    KTCoreDataExt& newData = newDataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& newTestData = newDataHandle->Of< KTTestDataExt >();
    {
        // create and open an archive for input
        std::ifstream fileIn("test_serialization_output.txt");

        boost::archive::text_iarchive iArchive(fileIn);
        // read class state from archive
        iArchive >> newData >> newTestData;
        // archive and stream closed when destructors are called
    }

    KTINFO(testlog, "Printing data structure -- after loading");
    printer.PrintDataStructure(newDataHandle);

    KTINFO(testlog, "KTCoreData counter: " << data.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << testData.GetIsAwesome());

    return 0;
}



