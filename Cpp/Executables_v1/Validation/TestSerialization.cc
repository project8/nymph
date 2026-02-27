/*
 * TestSerialization.cc
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#include "KTLogger.hh"

#include "KTTestData.hh"

#include "cereal/archives/json.hpp"

#include <fstream>

LOGGER( testlog, "TestSerialization" );

using namespace Nymph;

int main()
{
    const std::string filename( "test_serialization_output.json" );

    // save data to archive
    {
        KTCoreData coreData;
        KTTestData testData;
        KTTestBaseData testBaseData;

        // change data values from their defaults
        coreData.SetCounter( 5 );
        testData.SetIsAwesome( true );
        testBaseData.SetFunniness( 923 );

        KTINFO( testlog, "KTCoreData counter: " << coreData.GetCounter() );
        KTINFO( testlog, "KTTestData is-awesome: " << testData.GetIsAwesome() );
        KTINFO( testlog, "KTTestBaseData funniness: " << testBaseData.GetFunniness() );

        // create and open a file for output
        std::ofstream fileOut( filename );

        cereal::JSONOutputArchive archiveOut( fileOut );

        KTINFO( testlog, "Writing data to JSON archive" );
        archiveOut( CEREAL_NVP(coreData), CEREAL_NVP(testData), CEREAL_NVP(testBaseData) );

        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    {
        KTCoreData newCoreData;
        KTTestData newTestData;
        KTTestBaseData newTestBaseData;

         // create and open an archive for input
        std::ifstream fileIn( filename );

        cereal::JSONInputArchive archiveIn( fileIn );

        KTINFO( testlog, "Reading data from JSON archive" );
        archiveIn( newCoreData, newTestData, newTestBaseData );

        KTINFO( testlog, "KTCoreData counter: " << newCoreData.GetCounter() );
        KTINFO( testlog, "KTTestData is-awesome: " << newTestData.GetIsAwesome() );
        KTINFO( testlog, "KTTestBaseData funniness: " << newTestBaseData.GetFunniness() );

        // archive and stream closed when destructors are called
    }

    return 0;
}

