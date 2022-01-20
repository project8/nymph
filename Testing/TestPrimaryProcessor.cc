/*
 * TestPrimaryProcessor.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

#include "logger.hh"

#include "catch.hpp"

LOGGER( testlog, "TestPrimaryProcessor" );

REGISTER_PROCESSOR( Nymph, TestPrimaryProc, "test-primary" );


TEST_CASE( "primary_processor", "[primary_processor]" )
{
    using namespace Nymph;

    //SharedControl::get_instance()->Reset();

    TestPrimaryProc tester;
    REQUIRE_FALSE( tester.ExceptionPtr() );

    // SignalNewValue test
    tester.SetTestSelection( TestPrimaryProc::TestType::SignalNewValue );
    tester.ConnectASlot( "value", tester, "value" );
    tester();
    REQUIRE( tester.GetValue() == tester.GetNewValue() );

    // WaitTwoSec test
    // -- no test implemented here right now --
    //tester.SetTestSelection( TestPrimaryProc::TestType::WaitTwoSec );

    // ThrowExcept test
    tester.SetTestSelection( TestPrimaryProc::TestType::ThrowExcept );
    REQUIRE_NOTHROW( tester() ); // the exception should be caught by operator() and stored as the exception ptr
    REQUIRE( tester.ExceptionPtr() ); // the exception ptr should now be set
    try
    {
        std::rethrow_exception( tester.ExceptionPtr() );
    }
    catch( const scarab::base_exception& e )
    {
        PrintException( e );
        REQUIRE( std::string(e.what()) == std::string("PrimaryProcessor test function: throw Exception") );
    }
    
    

    //REQUIRE( tester.ValueSig().GetControl() == &control );
}
