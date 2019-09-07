/*
 * SignalSlotBase.cc
 *
 *  Created on: Sept 7, 2019
 *      Author: N.S. Oblath
 */

#include "SignalSlotBase.hh"

namespace Nymph
{
        SlotBase::SlotBase( const std::string& name ) :
            fName( name )
    {}

    SlotBase::~SlotBase()
    {}

    SignalBase::SignalBase( const std::string& name ) :
            fName( name )
    {}

    SignalBase::~SignalBase()
    {}

}
