/*
 * TestServiceClasses.hh
 *
 *  Created on: Dec 28, 2023
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_TESTING_SERVICECLASSES_HH_
#define NYMPH_TESTING_SERVICECLASSES_HH_

#include "Service.hh"

namespace NymphTesting
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestService : public Nymph::Service
    {
        public:
            TestService( const std::string& name = "test" );

            virtual ~TestService();

            void Configure( const scarab::param_node& node );

            MEMVAR( int, Value );
            MEMVAR( int, SecondValue );
            MEMVAR_REF( std::string, StringValue );
    };

} /* namespace NymphTesting */

#endif
