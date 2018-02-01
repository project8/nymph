/*
 *	TestSerialWriter.cc
 *
 *	Created on: Feb 1, 2018
 * 		Author: E Zayas
 *
*/

#include "KTLogger.hh"
#include "KTSerialWriter.hh"
#include "KTTestData.hh" 

LOGGER( testlog, "TestSerialWriter" );

using namespace Nymph;

int main()
{
	// Create the test data
	KTTestData testData;
	testData.SetIsAwesome( true ); // because it isn't by default :(

	KTINFO( testlog, "KTTestData is awesome? " << testData.GetIsAwesome() );

	// Create the writer
	KTSerialWriter writer;
	writer.SetFileName( "test_serial_writer_output.json" );
	writer.Initialize();

	writer.SlotFunction< KTTestData >( testData );

	KTINFO( testlog, "Validation script concluded" );
	return 0;
}
