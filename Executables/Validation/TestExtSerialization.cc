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

// register the data classes so they can be serialized from base-class pointers.
// json archive is already included, so it has been registered before these classes.
CEREAL_REGISTER_TYPE( KTCoreDataExt );
CEREAL_REGISTER_TYPE( KTTestDataExt );

int main()
{
    const std::string filename( "test_serialization_output.json" );

    // save data to archive
    {
        std::shared_ptr< KTCoreDataExt > coreData = std::make_shared< KTCoreDataExt >();
        std::shared_ptr< KTTestDataExt > testData = coreData->Share< KTTestDataExt >();

        // change data values from their defaults
        coreData->SetCounter( 5 );
        testData->SetIsAwesome( true );

        KTINFO( testlog, "KTCoreData counter: " << coreData->GetCounter() );
        KTINFO( testlog, "KTTestData is-awesome: " << testData->GetIsAwesome() );

        // create and open a file for output
        std::ofstream fileOut( filename );

        cereal::JSONOutputArchive archiveOut( fileOut );

        KTINFO( testlog, "Writing data to JSON archive" );
        archiveOut( coreData );

        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    {
        std::shared_ptr< KTCoreDataExt > newCoreData = std::make_shared< KTCoreDataExt >();

         // create and open an archive for input
        std::ifstream fileIn( filename );

        cereal::JSONInputArchive archiveIn( fileIn );

        KTINFO( testlog, "Reading data from JSON archive" );
        archiveIn( newCoreData );

        std::shared_ptr< KTTestDataExt > newTestData = newCoreData->Share< KTTestDataExt >();

        KTINFO(testlog, "KTCoreData counter: " << newCoreData->GetCounter());
        KTINFO(testlog, "KTTestData is-awesome: " << newTestData->GetIsAwesome());

        // archive and stream closed when destructors are called
    }

    return 0;
}

