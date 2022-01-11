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
    
}
