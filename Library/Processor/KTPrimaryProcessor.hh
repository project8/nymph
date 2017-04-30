/*
 * KTPrimaryProcessor.hh
 *
 *  Created on: Oct 10, 2012
 *      Author: nsoblath
 */

#ifndef KTPRIMARYPROCESSOR_HH_
#define KTPRIMARYPROCESSOR_HH_

#include "KTProcessor.hh"

#include "KTLogger.hh"

#include <future>

namespace Nymph
{

    class KTPrimaryProcessor : public KTProcessor
    {
        public:
            KTPrimaryProcessor( const std::string& name = "default-primary-processor-name" );
            virtual ~KTPrimaryProcessor();

        public:
            /// Callable function used by boost::thread
            virtual void operator()( std::promise< KTDataPtr >& promise );

        public:
            /// Starts the  main action of the processor
            virtual bool Run( std::promise< KTDataPtr >& promise ) = 0;

    };

} /* namespace Nymph */
#endif /* KTPRIMARYPROCESSOR_HH_ */
