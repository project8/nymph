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

    KTTestPrimaryProcessor tpp;
    KTTestProcessorB tp;

    KTINFO( testpplog, "Connecting the-signal to first-slot" );
    try
    {
        tpp.ConnectASlot( "the-signal", &tp, "first-slot", 20 );
    }
    catch( std::exception& e )
    {
        KTERROR( testpplog, "A problem occurred while connecting the signal and slots:\n" << e.what() );
        return -1;
    }

    KTINFO( testpplog, "Running the primary processor" )

    if( ! tpp.Run() )
    {
        KTERROR( testpplog, "Something went wrong while running the primary processor" );
        return -1;
    }

    KTINFO( testpplog, "Tests complete" );

    return 0;
}
