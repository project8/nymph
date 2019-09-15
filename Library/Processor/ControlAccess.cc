/*
 * ControlAccess.cc
 *
 *  Created on: Sept 15, 2019
 *      Author: N.S. Oblath
 */

#include "ControlAccess.hh"


namespace Nymph
{
    ReturnBufferBase::ReturnBufferBase()
    {}

    ReturnBufferBase::~ReturnBufferBase()
    {}


    ControlAccess::ControlAccess() :
            fReturn( new ReturnBuffer< int >() )
    {}

    ControlAccess::~ControlAccess()
    {}


} /* namespace Nymph */
