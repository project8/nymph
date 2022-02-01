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

    Exception blankEx;
    REQUIRE( blankEx.what() == std::string() );
    REQUIRE( blankEx.filename().empty() );
    REQUIRE( blankEx.line() == 0 );

    Exception testEx( "some_file", 1 );
    REQUIRE( testEx.filename() == "some_file" );
    REQUIRE( testEx.line() == 1 );

    std::string testMessage("test message 2: ");
    int testInt = 5;
    testEx << testMessage << testInt;
    REQUIRE( testEx.what() == testMessage + std::to_string(testInt) );

    /*EXCEPT_HERE( Exception testEx );*/ 
    // uses of __LINE__ need to be on the same line
    testEx( __FILE__, __LINE__ ); std::string recordFile( __FILE__ ); int recordLine( __LINE__ );
    REQUIRE( testEx.filename() == recordFile );
    REQUIRE( testEx.line() == recordLine );

    EXCEPT_HERE( testEx ); recordFile = std::string(__FILE__); recordLine = __LINE__;
    REQUIRE( testEx.filename() == recordFile );
    REQUIRE( testEx.line() == recordLine );
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
    
    std::string recordFile;
    int recordLine = 0;

    try
    {
        recordFile = std::string(__FILE__); recordLine = __LINE__;
        throw Exception( __FILE__, __LINE__ ) << "test message";
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == recordFile );
        REQUIRE( e.line() == recordLine + 1 );  // line number was recorded just before the throw
    }
    
    // Give CREATE_EXCEPT_HERE the class name
    try
    {
        recordFile = std::string(__FILE__); recordLine = __LINE__;
        throw CREATE_EXCEPT_HERE( Exception );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string() );
        REQUIRE( e.filename() == recordFile );
        REQUIRE( e.line() == recordLine + 1 );  // line number was recorded just before the throw
    }
    
    // Give EXCEPT_HERE an exception
    try
    {
        recordFile = std::string(__FILE__); recordLine = __LINE__;
        throw EXCEPT_HERE( Exception() << "test message" );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == recordFile );
        REQUIRE( e.line() == recordLine + 1 );  // line number was recorded just before the throw
    }
    
    // THROW macro
    try
    {
        recordFile = std::string(__FILE__); recordLine = __LINE__;
        THROW_EXCEPT_HERE( Exception() << "test message" );
    }
    catch(const Exception& e)
    {
        REQUIRE( e.what() == std::string("test message") );
        REQUIRE( e.filename() == recordFile );
        REQUIRE( e.line() == recordLine + 1 );  // line number was recorded just before the throw
    }
    
}

const unsigned layers = 5;
const std::string firstThrowText( "First throw" );
const std::string nestedThrowText( "nested throw at layer " );

void CatchThrow() 
{
    //const static unsigned layers = 5;
    static unsigned count = 0;

    try
    {
        if( count == layers )
        {
            throw CREATE_EXCEPT_HERE( Nymph::Exception ) << firstThrowText;
        }
        else
        {
            ++count;
            CatchThrow();
            return;
        }
    }
    catch(const Nymph::Exception& e)
    {
        if( count == layers )
        {
            REQUIRE( e.what() == firstThrowText );
        }
        else
        {
            std::string what( e.what() );
            REQUIRE( what.substr(0, nestedThrowText.size()) == nestedThrowText );
        }
        THROW_NESTED_EXCEPT_HERE( Nymph::Exception() << nestedThrowText << count-- ) ;
    }
}

void PrintExceptionWithTests( const Nymph::Exception& e, unsigned count = 0 )
{
    std::string prefix( count, ' ' );
    LINFO( testlog, prefix << "Thrown at: " << e.where() );
    LINFO( testlog, prefix << e.what() );
    if( count == layers + 1 )
    {
        REQUIRE( e.what() == firstThrowText );
    }
    else
    {
        std::string what( e.what() );
        REQUIRE( what.substr(0, nestedThrowText.size()) == nestedThrowText );
    }

    try
    {
        std::rethrow_if_nested( e );
    }
    catch(const Nymph::Exception& e)
    {
        PrintExceptionWithTests( e, ++count );
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

/* removed for trying home-grown exceptions, 1/10/22
    auto throwException = []() {
        BOOST_THROW_EXCEPTION( Exception() << "Test exception" << eom );
    };

    REQUIRE_THROWS_AS( throwException(), Exception );

    try
    {
        throwException();
    }
    catch(const Exception& e)
    {
        LWARN( testlog, diagnostic_information(e) );
    }
*/  
