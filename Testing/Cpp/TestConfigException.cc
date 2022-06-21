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

    std::string tceConfigStr(
        "- type: test-primary\n"
        "  name: testprimary-1\n"
        "- type: test-primary\n"
        "  name: testprimary-2\n"
        "- type: test-proc\n"
        "  name: testproc-1\n"
    );

    scarab::param_translator tceTranslator;
    auto tceConfig = tceTranslator.read_string( tceConfigStr, "yaml" );

    std::string tceConfigAsString = tceConfig->to_string();

    ConfigException tceException = ConfigException( *tceConfig );
    std::string tceExWhat( tceException.what() );

    using Catch::Matchers::EndsWith;

    REQUIRE_THAT( tceExWhat, EndsWith(tceConfigAsString) );
}
