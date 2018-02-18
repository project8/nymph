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

    class KTJSONWriterValidation : public KTJSONWriterTypist
    {
        public:
            KTJSONWriterValidation();
            virtual ~KTJSONWriterValidation();

            virtual void RegisterSlots();

        private:
            KTSlotData< void, KTTestDataExt > fTestDataSlot;

    };

} /* namespace Nymph */

#endif /* NYMPH_KTJSONWRITERVALIDATION_HH_ */
