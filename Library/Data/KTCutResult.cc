/*
 * KTCutResult.cc
 *
 *  Created on: Jun 11, 2017
 *      Author: obla999
 */

#include "KTCutResult.hh"

namespace Nymph
{
    KTCutResultCore::KTCutResultCore() :
            fState(false)
    {}

    KTCutResultCore::~KTCutResultCore()
    {}

    template<>
    bool KTExtensibleStructCore< KTCutResultCore >::fDoExtensibleSerialize = true;

} /* namespace Nymph */
