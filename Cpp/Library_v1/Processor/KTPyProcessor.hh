/*
 * KTPyProcessor.hh
 *
 *  Created on: Jan 29, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTPYPROCESSOR_HH_
#define NYMPH_KTPYPROCESSOR_HH_

#include "KTProcessor.hh"

namespace Nymph
{

    class KTPyProcessor : public KTProcessor
    {
        public:
            using KTProcessor::KTProcessor; // inherit constructors

            // for now, override KTConfigurable::Configure(const scarab::param_node&) with a non-functional function
            bool Configure(const scarab::param_node& )
            {
                return true;
            }
    };



} /* namespace Nymph */

#endif /* NYMPH_KTPYPROCESSOR_HH_ */
