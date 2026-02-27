/*
 * KTPrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: nsoblath
 */

#include "KTPrimaryProcessor.hh"

#include "KTException.hh"
#include "KTLogger.hh"

namespace Nymph
{
    KTLOGGER( proclog, "KTPrimaryProcessor" );

    KTPrimaryProcessor::KTPrimaryProcessor( std::initializer_list< std::string > signals, const std::string& name ) :
            KTProcessor( name ),
            fSignalsEmitted( signals ),
            fThreadRef(),
            fDoBreakpoint(false)
    {

    }

    KTPrimaryProcessor::~KTPrimaryProcessor()
    {
    }

    void KTPrimaryProcessor::operator ()( std::shared_ptr< KTThreadReference > ref, boost::condition_variable& startedCV, bool& startedFlag )
    {
        fThreadRef = ref;

        // pass updated thread reference to downstream slots
        for( auto sigIt = fSignalsEmitted.begin(); sigIt != fSignalsEmitted.end(); ++sigIt )
        {
            // loop over all processor:slots called by this signal
            auto sigConnRange = fSigConnMap.equal_range( *sigIt );
            for( SigConnMapCIt sigConnIt = sigConnRange.first; sigConnIt != sigConnRange.second; ++sigConnIt )
            {
                // pass the update on to the connected-to processor
                sigConnIt->second.first->PassThreadRefUpdate( sigConnIt->second.second, fThreadRef );
            }
        }

        startedFlag = true;
        startedCV.notify_all();

        // go!
        try
        {
            if( ! Run() )
            {
                KTERROR( proclog, "An error occurred during processor running." );
                THROW_THREADREF_EXCEPTION( fThreadRef, KTException() << "An error occurred during processor running" );
            }
            else
            {
                fThreadRef->SetReturnValue( KTDataHandle() );
            }
        }
        catch( boost::exception& e )
        {
            fThreadRef->SetReturnException( boost::current_exception() );
        }
        return;
    }

} /* namespace Nymph */
