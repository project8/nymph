/*
 * KTPrimaryProcessor.hh
 *
 *  Created on: Oct 10, 2012
 *      Author: nsoblath
 */

#ifndef KTPRIMARYPROCESSOR_HH_
#define KTPRIMARYPROCESSOR_HH_

#include "KTProcessor.hh"

#include "KTData.hh"
#include "KTThreadReference.hh"

#include <future>

namespace Nymph
{

    class KTPrimaryProcessor : public KTProcessor
    {
        public:
            KTPrimaryProcessor( std::initializer_list< std::string > signals, const std::string& name = "default-primary-processor-name" );
            virtual ~KTPrimaryProcessor();

        public:
            /// Callable function used by std::thread
            void operator()( KTThreadReference&& ref );

            /// Starts the  main action of the processor
            virtual bool Run() = 0;

            KTThreadReference* GetThreadRef();

            MEMBERVARIABLE( bool, DoBreakpoint );

        protected:
            std::vector< std::string > fSignalsEmitted;

            KTThreadReference fThreadRef;

    };

    inline KTThreadReference* KTPrimaryProcessor::GetThreadRef()
    {
        return &fThreadRef;
    }

} /* namespace Nymph */
#endif /* KTPRIMARYPROCESSOR_HH_ */
