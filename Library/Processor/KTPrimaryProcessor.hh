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
            KTPrimaryProcessor( const std::string& name = "default-primary-processor-name" );
            virtual ~KTPrimaryProcessor();

        public:
            /// Callable function used by std::thread
            void operator()();

            /// Starts the  main action of the processor
            virtual bool Run() = 0;

            void SetThreadRef( KTThreadReference&& ref );
            KTThreadReference* GetThreadRef();

        protected:
            KTThreadReference fThreadRef;


    };

    inline void KTPrimaryProcessor::SetThreadRef( KTThreadReference&& ref )
    {
        fThreadRef = std::move( ref );
        return;
    }

    inline KTThreadReference* KTPrimaryProcessor::GetThreadRef()
    {
        return &fThreadRef;
    }

} /* namespace Nymph */
#endif /* KTPRIMARYPROCESSOR_HH_ */
