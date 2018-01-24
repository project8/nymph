/*
 * TestSerialization.cc
 *
 *  Created on: Jun 12, 2017
 *      Author: obla999
 */

#include "KTLogger.hh"

#include "KTTestData.hh"

#include <boost/archive/polymorphic_text_oarchive.hpp>
#include <boost/archive/polymorphic_text_iarchive.hpp>

#include <fstream>

KTLOGGER(testlog, "TestSerialization");


using namespace Nymph;
using namespace std;

int main()
{
    // save data to archive
    {
        KTCoreData coreData;
        KTTestData testData;

        // change data values from their defaults
        coreData.SetCounter( 500 );
        testData.SetIsAwesome( true );

        KTINFO(testlog, "KTCoreData counter: " << coreData.GetCounter());
        KTINFO(testlog, "KTTestData is-awesome: " << testData.GetIsAwesome());

        // create and open a character archive for output
        std::ofstream fileOut("test_polymorphic_serialization_output.txt");

        boost::archive::polymorphic_text_oarchive oArchive(fileOut);
        // write data objects to archive
        KTDEBUG(testlog, "Archiving ptr to KTCoreData");
        oArchive << coreData;
        KTDEBUG(testlog, "Archiving ptr to KTTestData")
        oArchive << testData;

        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    {
        KTCoreData newData;
        KTTestData newTestData;

        // create and open an archive for input
        std::ifstream fileIn("test_polymorphic_serialization_output.txt");

        boost::archive::polymorphic_text_iarchive iArchive(fileIn);
        // read class state from archive
        KTDEBUG(testlog, "Loading core data");
        iArchive >> newData;
        KTDEBUG(testlog, "Loading test data");
        iArchive >> newTestData;

        KTINFO(testlog, "KTCoreData counter: " << newData.GetCounter());
        KTINFO(testlog, "KTTestData is-awesome: " << newTestData.GetIsAwesome());

        // archive and stream closed when destructors are called
    }

    return 0;
}
