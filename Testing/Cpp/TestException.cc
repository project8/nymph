/*
 * TestException.cc
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#include "Exception.hh"

#include "catch.hpp"

#include "logger.hh"

LOGGER( testlog, "TestException" );

TEST_CASE( "exception", "[utility]" )
{
    using namespace Nymph;

    Exception teBlankEx;
    REQUIRE( teBlankEx.what() == std::string() );
    REQUIRE( teBlankEx.filename().empty() );
    REQUIRE( teBlankEx.line() == 0 );

    Exception teTestEx( "some_file", 1 );
    REQUIRE( teTestEx.filename() == "some_file" );
    REQUIRE( teTestEx.line() == 1 );

    std::string teTestMessage("test message 2: ");
    int testInt = 5;
    teTestEx << teTestMessage << testInt;
    REQUIRE( teTestEx.what() == teTestMessage + std::to_string(testInt) );

    /*EXCEPT_HERE( Exception teTestEx );*/ 
    // uses of __LINE__ need to be on the same line
    teTestEx( __FILE__, __LINE__ ); std::string tdeRecordFile( __FILE__ ); int tdeRecordLine( __LINE__ );
    REQUIRE( teTestEx.filename() == tdeRecordFile );
    REQUIRE( teTestEx.line() == tdeRecordLine );

    EXCEPT_HERE( teTestEx ); tdeRecordFile = std::string(__FILE__); tdeRecordLine = __LINE__;
    REQUIRE( teTestEx.filename() == tdeRecordFile );
    REQUIRE( teTestEx.line() == tdeRecordLine );
}

TEST_CASE( "throw", "[utility]" )
{
    using namespace Nymph;

    try
    {
        throw Exception();
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string() );
        REQUIRE( e.filename().empty() );
        REQUIRE( e.line() == 0 );
    }
    
    std::string tdeRecordFile;
    int tdeRecordLine = 0;

    try
    {
        tdeRecordFile = std::string(__FILE__); tdeRecordLine = __LINE__;
        throw Exception( __FILE__, __LINE__ ) << "test message";
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == tdeRecordFile );
        REQUIRE( e.line() == tdeRecordLine + 1 );  // line number was recorded just before the throw
    }
    
    // Give CREATE_EXCEPT_HERE the class name
    try
    {
        tdeRecordFile = std::string(__FILE__); tdeRecordLine = __LINE__;
        throw CREATE_EXCEPT_HERE( Exception );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string() );
        REQUIRE( e.filename() == tdeRecordFile );
        REQUIRE( e.line() == tdeRecordLine + 1 );  // line number was recorded just before the throw
    }
    
    // Give EXCEPT_HERE an exception
    try
    {
        tdeRecordFile = std::string(__FILE__); tdeRecordLine = __LINE__;
        throw EXCEPT_HERE( Exception() << "test message" );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == tdeRecordFile );
        REQUIRE( e.line() == tdeRecordLine + 1 );  // line number was recorded just before the throw
    }
    
    // THROW macro
    try
    {
        tdeRecordFile = std::string(__FILE__); tdeRecordLine = __LINE__;
        THROW_EXCEPT_HERE( Exception() << "test message" );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == tdeRecordFile );
        REQUIRE( e.line() == tdeRecordLine + 1 );  // line number was recorded just before the throw
    }
    
}

const unsigned tdeLayers = 5;
const std::string tdeFirstThrowText( "First throw" );
const std::string tdeNestedThrowText( "nested throw at layer " );

void CatchThrow() 
{
    //const static unsigned tdeLayers = 5;
    static unsigned tdeCount = 0;

    try
    {
        if( tdeCount == tdeLayers )
        {
            throw CREATE_EXCEPT_HERE( Nymph::Exception ) << tdeFirstThrowText;
        }
        else
        {
            ++tdeCount;
            CatchThrow();
            return;
        }
    }
    catch(const Nymph::Exception& e)
    {
        if( tdeCount == tdeLayers )
        {
            REQUIRE( e.what() == tdeFirstThrowText );
        }
        else
        {
            std::string tdeWhat( e.what() );
            REQUIRE( tdeWhat.substr(0, tdeNestedThrowText.size()) == tdeNestedThrowText );
        }
        THROW_NESTED_EXCEPT_HERE( Nymph::Exception() << tdeNestedThrowText << tdeCount-- ) ;
    }
}

void PrintExceptionWithTests( const Nymph::Exception& e, unsigned tdeCount = 0 )
{
    std::string prefix( tdeCount, ' ' );
    LINFO( testlog, prefix << "Thrown at: " << e.where() );
    LINFO( testlog, prefix << e.what() );
    if( tdeCount == tdeLayers + 1 )
    {
        REQUIRE( e.what() == tdeFirstThrowText );
    }
    else
    {
        std::string tdeWhat( e.what() );
        REQUIRE( tdeWhat.substr(0, tdeNestedThrowText.size()) == tdeNestedThrowText );
    }

    try
    {
        std::rethrow_if_nested( e );
    }
    catch(const Nymph::Exception& e)
    {
        PrintExceptionWithTests( e, ++tdeCount );
    }
    return;
}

TEST_CASE( "nested_throw", "[utility]" )
{
    using namespace Nymph;

    try
    {
        CatchThrow();
    }
    catch(const Exception& e)
    {

        PrintExceptionWithTests(e);
    }
    
}
