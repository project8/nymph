/*
 * KTRegisterNymphValidationExtData.hh
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTREGISTERNYMPHVALIDATIONEXTDATA_HH_
#define NYMPH_KTREGISTERNYMPHVALIDATIONEXTDATA_HH_

#include "KTRegisterNymphExtData.hh"

#include "KTTestData.hh"

CEREAL_REGISTER_TYPE( Nymph::KTTestDataExt );
CEREAL_REGISTER_TYPE( Nymph::KTTestDerived1DataExt );
CEREAL_REGISTER_TYPE( Nymph::KTTestDerived2DataExt );

#endif /* NYMPH_KTREGISTERNYMPHVALIDATIONEXTDATA_HH_ */
