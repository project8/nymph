/*
 * TestPrimaryProcessorNoThreading.cc
 *
 *  Created on: May 5, 2017
 *      Author: N.S. Oblath
 */

#include "KTTestPrimaryProcessor.hh"
#include "KTTestProcessor.hh"

#include "KTLogger.hh"

using namespace Nymph;

KTLOGGER( testpplog, "TestPrimaryProcessorNoThreading" )

int main()
{

    try
    {
        KTTestPrimaryProcessor tpp;
        KTTestProcessorB tp;

        KTINFO( testpplog, "Connecting the-signal to first-slot" );
        tpp.ConnectASlot( "the-signal", &tp, "first-slot", 20 );

        KTINFO( testpplog, "Running the primary processor" )

        if( ! tpp.Run() )
        {
            KTERROR( testpplog, "Something went wrong while running the primary processor" );
            return -1;
        }

        KTINFO( testpplog, "Tests complete" );
    }
    catch( boost::exception& e )
    {
        KTERROR( testpplog, "An unexpected exception was caught: " << diagnostic_information( e ) );
        return -1;
    }

    return 0;
}
