/*
 * KTSignalWrapper.cc
 *
 *  Created on: Aug 8, 2012
 *      Author: nsoblath
 */

#include "KTSignalWrapper.hh"

namespace Nymph
{

    KTSignalWrapper::KTSignalWrapper() :
            fSignalWrapper(NULL)
    {
    }

    KTSignalWrapper::~KTSignalWrapper()
    {
        delete fSignalWrapper;
    }

} /* namespace Nymph */
