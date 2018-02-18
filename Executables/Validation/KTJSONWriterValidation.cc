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
    KT_REGISTER_TYPE_WRITER( KTJSONWriter, KTJSONTypeWriter, Validation );

    KTJSONTypeWriterValidation::KTJSONTypeWriterValidation( KTJSONWriter* writer) :
            KTJSONTypeWriter( writer ),
            fTestSlot( "test", fWriter, &KTJSONWriter::WriteData< KTTestDataExt > ),
            fTestDerived1Slot( "test-derived-1", fWriter, &KTJSONWriter::WriteData< KTTestDerived1DataExt > ),
            fTestDerived2Slot( "test-derived-2", fWriter, &KTJSONWriter::WriteData< KTTestDerived2DataExt > )
    {
        std::cout << "Created a KTJSONWriterValidation" << std::endl;
    }

    KTJSONTypeWriterValidation::~KTJSONTypeWriterValidation()
    {
    }

    void KTJSONTypeWriterValidation::RegisterSlots()
    {
    }

} /* namespace Nymph */
