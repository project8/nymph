/*
 * TestControlAccesss.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */


#include "ControlAccess.hh"

#include "catch.hpp"


TEST_CASE( "control_access", "[control_access]" )
{
    using namespace Nymph;

    ControlAccess control;

    int intReturn = 5;

    REQUIRE_THROWS( control.GetReturn< int >() );

    control.SetReturn< int >( intReturn );

    REQUIRE( control.GetReturn< int >() == intReturn );
    REQUIRE_THROWS( control.GetReturn< unsigned >() == intReturn );

}