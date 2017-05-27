/*
 * KTSlotWrapper.cc
 *
 *  Created on: Feb 10, 2012
 *      Author: nsoblath
 */

#include "KTSlotWrapper.hh"

namespace Nymph
{

    KTSlotWrapper::KTSlotWrapper() :
            fSlotWrapper(NULL),
            fConnection(),
            fThreadRef(),
            fDoBreakpoint(false)
    {
    }

    KTSlotWrapper::~KTSlotWrapper()
    {
        this->Disconnect();
        delete fSlotWrapper;
    }

} /* namespace Nymph */
