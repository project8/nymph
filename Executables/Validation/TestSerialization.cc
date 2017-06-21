/*
 * TestSerialization.cc
 *
 *  Created on: Jun 12, 2017
 *      Author: obla999
 */

#include "KTLogger.hh"

#include "KTTestData.hh"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>

KTLOGGER(testlog, "TestSerialization");


using namespace Nymph;
using namespace std;

int main()
{
    KTCoreData coreData;
    KTTestData testData;

    // change data values from their defaults
    coreData.SetCounter( 5 );
    testData.SetIsAwesome( true );

    KTINFO(testlog, "KTCoreData counter: " << coreData.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << testData.GetIsAwesome());

    // save data to archive
    {
        // create and open a character archive for output
        std::ofstream fileOut("test_serialization_output.txt");

        boost::archive::text_oarchive oArchive(fileOut);
        // write data objects to archive
        KTDEBUG(testlog, "Archiving ptr to KTCoreData");
        oArchive << coreData;
        KTDEBUG(testlog, "Archiving ptr to KTTestData")
        oArchive << testData;
        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    KTCoreData newData;
    KTTestData newTestData;
    {
        // create and open an archive for input
        std::ifstream fileIn("test_serialization_output.txt");

        boost::archive::text_iarchive iArchive(fileIn);
        // read class state from archive
        KTDEBUG(testlog, "Loading core data");
        iArchive >> newData;
        KTDEBUG(testlog, "Loading test data");
        iArchive >> newTestData;
        // archive and stream closed when destructors are called
    }

    KTINFO(testlog, "KTCoreData counter: " << coreData.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << testData.GetIsAwesome());

    return 0;
}
