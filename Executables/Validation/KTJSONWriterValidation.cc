/*
 * KTJSONWriterValidation.cc
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#include "KTJSONWriterValidation.hh"

#include "KTTestData.hh"

namespace Nymph
{

    KTJSONWriterValidation::KTJSONWriterValidation() :
            KTJSONWriterTypist(),
            fTestDataSlot( "test", fWriter, &KTJSONWriter::WriteData< KTTestDataExt > )
    {
    }

    KTJSONWriterValidation::~KTJSONWriterValidation()
    {
    }

    void KTJSONWriterValidation::RegisterSlots()
    {
    }

} /* namespace Nymph */
