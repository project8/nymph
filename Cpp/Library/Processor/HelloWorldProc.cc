/*
 * HelloWorldCpp.cc
 *
 *  Created on: Sep 22, 2025
 *      Author: N.S. Oblath
 */

#include "HelloWorldProc.hh"

#include "logger.hh"

REGISTER_PROCESSOR( Nymph, HelloWorldCpp, "hello-world-cpp" );

LOGGER( hwlog, "hello-world-proc")

namespace Nymph
{

    HelloWorldCpp::HelloWorldCpp( const std::string& name ) :
            Nymph::Processor( name ),
            fHelloSig( "hello", this ),
            fHelloSlot( "hello", this, &HelloWorldCpp::SayHello )
    {}

    HelloWorldCpp::~HelloWorldCpp()
    {}

    void HelloWorldCpp::SayHello()
    {
        LPROG( hwlog, "Hello, world" );
        return;
    }
}
