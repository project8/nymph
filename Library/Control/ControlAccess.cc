/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"

#include "logger.hh"

//#include <boost/thread/locks.hpp>
//#include <boost/thread/lock_types.hpp>

LOGGER( contlog, "ControlAccess" );

namespace Nymph
{
    SharedControl::SharedControl() :
    {}

    SharedControl::~SharedControl()
    {}




    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() )//,
            //fControl()
    {}

    ControlAccess::~ControlAccess()
    {}

} /* namespace Nymph */
