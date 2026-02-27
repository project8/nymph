/*
 * TestServiceClasses.cc
 *
 *  Created on: Dec 28, 2023
 *      Author: N.S. Oblath
 */

#include "TestServiceClasses.hh"

#include "ConfigException.hh"

REGISTER_SERVICE( NymphTesting, TestService, "test-svc" );

namespace NymphTesting
{
    TestService::TestService( const std::string& name ) :
            Nymph::Service( name ),
            fValue( 0 ),
            fSecondValue( 0 )
    {}

    TestService::~TestService()
    {}

    void TestService::Configure( const scarab::param_node& node )
    {
        fValue = node.get_value( "value", fValue );

        // we'll test configuration error throwing by having a string with restricted values
        if( node.has("string") )
        {
            if( node["string"]().as_string() == "ok value 1" ) fStringValue = "ok 1";
            else if( node["string"]().as_string() == "ok value 2" ) fStringValue = "ok 2";
            else THROW_EXCEPT_HERE( Nymph::ConfigException( node ) << "Invalid choice for \"string\"" );
        }

        return;
    }

} /* namespace NymphTesting */
