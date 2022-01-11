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
    REQUIRE( blankEx.AtFilename() == "unknown" );
    REQUIRE( blankEx.GetAtLineNumber() == 0 );

    Exception testEx( "some_file", 1 );
    REQUIRE( testEx.AtFilename() == "some_file" );
    REQUIRE( testEx.GetAtLineNumber() == 1 );

    std::string testMessage("test message 2: ");
    int testInt = 5;
    testEx << testMessage << testInt;
    REQUIRE( testEx.what() == testMessage + std::to_string(testInt) );

    /*EXCEPT_HERE( Exception testEx );*/ 
    // uses of __LINE__ need to be on the same line
    testEx( __FILE__, __LINE__ ); std::string recordFile( __FILE__ ); int recordLine( __LINE__ );
    LWARN( testlog, "recorded file: " << recordFile );
    LWARN( testlog, "recorded line: " << recordLine );
    REQUIRE( testEx.AtFilename() == recordFile );
    REQUIRE( testEx.GetAtLineNumber() == recordLine );

    EXCEPT_HERE( testEx ); recordFile = std::string(__FILE__); recordLine = __LINE__;
    REQUIRE( testEx.AtFilename() == recordFile );
    REQUIRE( testEx.GetAtLineNumber() == recordLine );
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
        REQUIRE( e.AtFilename() == "unknown" );
        REQUIRE( e.GetAtLineNumber() == 0 );
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
        REQUIRE( e.AtFilename() == recordFile );
        REQUIRE( e.GetAtLineNumber() == recordLine + 1 );  // line number was recorded just before the throw
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
        REQUIRE( e.AtFilename() == recordFile );
        REQUIRE( e.GetAtLineNumber() == recordLine + 1 );  // line number was recorded just before the throw
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
        REQUIRE( e.AtFilename() == recordFile );
        REQUIRE( e.GetAtLineNumber() == recordLine + 1 );  // line number was recorded just before the throw
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
        REQUIRE( e.AtFilename() == recordFile );
        REQUIRE( e.GetAtLineNumber() == recordLine + 1 );  // line number was recorded just before the throw
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
