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

#include <boost/thread/condition_variable.hpp>

#include <atomic>

namespace Nymph
{

    class KTPrimaryProcessor : public KTProcessor
    {
        public:
            KTPrimaryProcessor( std::initializer_list< std::string > signals, const std::string& name = "default-primary-processor-name" );
            virtual ~KTPrimaryProcessor();

        public:
            /// Callable function used by std::thread
            void operator()( std::shared_ptr< KTThreadReference > ref, boost::condition_variable& startedCV, bool& startedFlag );

            /// Starts the  main action of the processor
            virtual bool Run() = 0;

            std::shared_ptr< KTThreadReference > GetThreadRef();

            MEMBERVARIABLE( bool, DoBreakpoint );

        protected:
            std::vector< std::string > fSignalsEmitted;

            std::shared_ptr< KTThreadReference > fThreadRef;

    };

    inline std::shared_ptr< KTThreadReference > KTPrimaryProcessor::GetThreadRef()
    {
        return fThreadRef;
    }

} /* namespace Nymph */
#endif /* KTPRIMARYPROCESSOR_HH_ */
