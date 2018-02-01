/*
 *	TestSerialWriter.cc
 *
 *	Created on: Feb 1, 2018
 * 		Author: E Zayas
 *
*/

#include "KTJSONWriter.hh"
#include "KTJSONReader.hh"

#include "KTTestData.hh" 

KTLOGGER( testlog, "TestJSONWriter" );

using namespace Nymph;

int main()
{
    std::string filename( "test_json_writer_output.json" );

    {
        // Create the test data
        KTTestData testData;
        testData.SetIsAwesome( true ); // because it isn't by default :(

        KTINFO( testlog, "KTTestData is awesome? " << testData.GetIsAwesome() );

        // Create the writer
        KTJSONWriter writer;
        writer.SetFilename( filename );

        KTINFO( testlog, "Writing data" );

        writer.WriteData< KTTestData >( testData );

        // destruction of the stream and archive (in the writer's destructor) properly closes the file
    }

    {
        KTTestData newTestData;

        KTJSONReader reader;
        reader.SetFilename( filename );

        reader.ReadData< KTTestData >( newTestData );

        KTINFO( testlog, "Is the read data awesome? " << newTestData.GetIsAwesome() );
    }

    KTINFO( testlog, "Validation script concluded" );
	return 0;
}
