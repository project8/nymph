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

    SECTION( "One Return" )
    {
        int intReturn = 5;

        REQUIRE_THROWS( control.GetReturn< int >() );

        control.SetReturn< int >( intReturn );

        REQUIRE( std::get< 0 >(control.GetReturn< int >()) == intReturn );
        REQUIRE_THROWS( control.GetReturn< unsigned >() );
    }

    SECTION( "Two Returns" )
    {
        int return1 = 5;
        double return2 = 10.;

        control.SetReturn< int, double >( return1, return2 );

        REQUIRE( std::get< 0 >(control.GetReturn< int, double >()) == return1 );
        REQUIRE( std::get< 1 >(control.GetReturn< int, double >()) == return2 );
    }

}