/*
 * KTRegisterNymphExtData.hh
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTREGISTERNYMPHEXTDATA_HH_
#define NYMPH_KTREGISTERNYMPHEXTDATA_HH_

/**
 * @file KTRegisterNymphExtData.hh
 *
 * Register the extensible data objects that live in Nymph here.
 * This file should be included with registration headers packages using Nymph's extensible data setup.
 *
 * This header should be included _after_ the relevant archives are registered (after #include'ing the headers, generally)
 *
 */


#include <cereal/types/polymorphic.hpp>

#include "KTCoreData.hh"

CEREAL_REGISTER_TYPE( Nymph::KTCoreDataExt );


#endif /* NYMPH_KTREGISTERNYMPHEXTDATA_HH_ */
