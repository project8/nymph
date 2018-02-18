/*
 * KTJSONWriterValidation.hh
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTJSONWRITERVALIDATION_HH_
#define NYMPH_KTJSONWRITERVALIDATION_HH_

#include "KTJSONWriter.hh"

namespace Nymph
{
    class KTTestDataExt;
    class KTTestDerived1DataExt;
    class KTTestDerived2DataExt;

    class KTJSONTypeWriterValidation : public KTJSONTypeWriter
    {
        public:
            KTJSONTypeWriterValidation( KTJSONWriter* writer );
            virtual ~KTJSONTypeWriterValidation();

            virtual void RegisterSlots();

        private:
            KTSlotData< void, KTTestDataExt > fTestSlot;
            KTSlotData< void, KTTestDerived1DataExt > fTestDerived1Slot;
            KTSlotData< void, KTTestDerived2DataExt > fTestDerived2Slot;

    };

} /* namespace Nymph */

#endif /* NYMPH_KTJSONWRITERVALIDATION_HH_ */
