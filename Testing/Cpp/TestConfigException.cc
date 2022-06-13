/*
 * TestConfigException.cc
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#include "ConfigException.hh"

#include "param_codec.hh"

#include "catch.hpp"


TEST_CASE( "config_exception", "[exception],[utility]" )
{
    using namespace Nymph;

    std::string config_str(
        "- type: test-primary\n"
        "  name: testprimary-1\n"
        "- type: test-primary\n"
        "  name: testprimary-2\n"
        "- type: test-proc\n"
        "  name: testproc-1\n"
    );

    scarab::param_translator translator;
    auto config = translator.read_string( config_str, "yaml" );

    std::string config_as_string = config->to_string();

    ConfigException exception = ConfigException( *config );
    std::string ex_what( exception.what() );

    using Catch::Matchers::EndsWith;

    REQUIRE_THAT( ex_what, EndsWith(config_as_string) );
}
