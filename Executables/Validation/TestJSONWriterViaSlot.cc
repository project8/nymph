/*
 * TestJSONWriterViaSlot.cc
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#include "KTJSONTypeWriterValidation.hh"
#include "KTTestDataPrimaryProcessor.hh"
#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER( testlog, "TestJSONWriterViaSlot" )

int main()
{

    try
    {
        KTPROG( testlog, "Testing single-data-object writing" );

        KTINFO( testlog, "Creating processors" );
        KTTestDataPrimaryProcessor tdpp;
        KTJSONWriter jw;

        KTINFO( testlog, "Configuring processors" );
        scarab::param_node jwConfig;
        jwConfig.add( "filename", scarab::param_value( "test_jsonwriter_slot_singleobj.json" ) );
        jw.Configure( jwConfig );

        KTINFO( testlog, "Connecting the-signal to first-slot" );
        tdpp.ConnectASlot( "test-derived-1", &jw, "test-derived-1", 20 );

        KTINFO( testlog, "Running the primary processor" )

        if( ! tdpp.Run() )
        {
            KTERROR( testlog, "Something went wrong while running the primary processor" );
            return -1;
        }

        KTINFO( testlog, "Test complete" );
    }
    catch( boost::exception& e )
    {
        KTERROR( testlog, "An unexpected exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    try
    {
        KTPROG( testlog, "Testing extensible-data-object writing" );

        KTINFO( testlog, "Creating processors" );
        KTTestDataPrimaryProcessor tdpp;
        KTJSONWriter jw;

        KTINFO( testlog, "Configuring processors" );
        scarab::param_node jwConfig;
        jwConfig.add( "filename", scarab::param_value( "test_jsonwriter_slot_extdata.json" ) );
        jw.Configure( jwConfig );

        KTINFO( testlog, "Connecting the-signal to first-slot" );
        tdpp.ConnectASlot( "test-derived-1", &jw, "ext-data", 20 );

        KTINFO( testlog, "Running the primary processor" )

        if( ! tdpp.Run() )
        {
            KTERROR( testlog, "Something went wrong while running the primary processor" );
            return -1;
        }

        KTINFO( testlog, "Test complete" );
    }
    catch( boost::exception& e )
    {
        KTERROR( testlog, "An unexpected exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}
