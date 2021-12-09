/*
 * PrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"

#include "QuitThread.hh"
#include "Exception.hh"

#include "logger.hh"

namespace Nymph
{
    LOGGER( proclog, "PrimaryProcessor" );

    PrimaryProcessor::PrimaryProcessor( const std::string& name ) :
            Processor( name )
    {}

    PrimaryProcessor::~PrimaryProcessor()
    {}

    void PrimaryProcessor::operator()()// ControlAccessPtr control ) // SharedControl is now accessed in Signal::operator(), so we don't have to pass it from signal to slot to signal, etc
    {
        // pass the control access pointer to every signal in the primary processor
        /*
        for( auto signalIt = fSignals.begin(); signalIt != fSignals.end(); ++signalIt )
        {
            signalIt->second->SetControlAcc( control );
        }
        */

        // go!
        try
        {
            if( ! Run() )
            {
                LERROR( proclog, "An error occurred during processor running." );
                //THROW_THREADREF_EXCEPTION( fThreadRef, KTException() << "An error occurred during processor running" );
            }
            else
            {
                LWARN( proclog, "Valid return" );
                //fThreadRef->SetReturnValue( KTDataHandle() );
            }
        }
        catch( QuitThread& e )
        {
            LDEBUG( proclog, "Thread quit from " << e.fFile << ", line " << e.fLine );
        }
        catch( boost::exception& e )
        {
            //fThreadRef->SetReturnException( boost::current_exception() );
        }
        return;
    }

} /* namespace Nymph */
