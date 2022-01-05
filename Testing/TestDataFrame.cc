/*
 * TestDataFrame.cc
 *
 *  Created on: Jan 5, 2022
 *      Author: N.S. Oblath
 */

#include "DataFrame.hh"

#include "catch.hpp"


TEST_CASE( "data_frame", "[data]" )
{
    using namespace Nymph;

    DataFrame frame;
    REQUIRE( frame.DataObjects().empty() );

}
