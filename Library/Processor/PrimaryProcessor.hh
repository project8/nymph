/*
 * PrimaryProcessor.hh
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_PRIMARYPROCESSOR_HH_
#define NYMPH_PRIMARYPROCESSOR_HH_

#include "Processor.hh"


namespace Nymph
{

    class PrimaryProcessor : public Processor
    {
        public:
            PrimaryProcessor( const std::string& name );
            virtual ~PrimaryProcessor();

        public:
            /// Callable function used by std::thread
            void operator()();// ControlAccessPtr control ); // SharedControl is now accessed in Signal::operator(), so we don't have to pass it from signal to slot to signal, etc

            /// Starts the  main action of the processor
            virtual void Run() = 0;

//            MEMVAR_REF( std::exception_ptr, ExceptionPtr );

    };


/*
    class PrimaryProcessor : public Processor
    {
        public:
            PrimaryProcessor( std::initializer_list< std::string > signals, const std::string& name = "default-primary-processor-name" );
            virtual ~PrimaryProcessor();

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

    inline std::shared_ptr< KTThreadReference > PrimaryProcessor::GetThreadRef()
    {
        return fThreadRef;
    }
*/

} /* namespace Nymph */

#endif /* NYMPH_PRIMARYPROCESSOR_HH_ */
