/*
 * TestJSONWriterViaSlot.cc
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#include "KTTestDataPrimaryProcessor.hh"
#include "KTJSONWriterValidation.hh"

#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER( testlog, "TestJSONWriterViaSlot" )

int main()
{

    try
    {
        KTINFO( testlog, "Creating processors" );
        KTTestDataPrimaryProcessor tdpp;
        KTJSONWriter jw;

        KTINFO( testlog, "Connecting the-signal to first-slot" );
        tdpp.ConnectASlot( "test-derived-1", &jw, "test-derived-1", 20 );

        KTINFO( testlog, "Running the primary processor" )

        if( ! tdpp.Run() )
        {
            KTERROR( testlog, "Something went wrong while running the primary processor" );
            return -1;
        }

        KTINFO( testlog, "Tests complete" );
    }
    catch( boost::exception& e )
    {
        KTERROR( testlog, "An unexpected exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}
