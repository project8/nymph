/*
 * use_catch.cc
 *
 *  Created on: Aug 14, 2018
 *      Author: N.S. Oblath
 */

#include "catch2/catch_session.hpp"

int main( int argc, char* argv[] ) 
{
    int result = Catch::Session().run( argc, argv );

    return result;
}
