/*
 * Service.cc
 *
 *  Created on: Dec 28, 2023
 *      Author: N.S. Oblath
 */

#include "Service.hh"

namespace Nymph
{
    //KTLOGGER(servicelog, "Service");

    Service::Service( const std::string& name ) :
            fName( name )
    {
    }

    Service::~Service()
    {}

} /* namespace Nymph */
