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
            Processor( name ),
            fExceptionPtr()
    {}

    PrimaryProcessor::~PrimaryProcessor()
    {}

    void PrimaryProcessor::operator()()// ControlAccessPtr control ) // SharedControl is now accessed in Signal::operator(), so we don't have to pass it from signal to slot to signal, etc
    {
        // go!
        try
        {
            Run();
        }
        catch( QuitThread& e )
        {
            LDEBUG( proclog, "Thread quit from " << e.fFile << ", line " << e.fLine );
        }
        catch( std::exception& e )
        {
            LERROR( proclog, "An error occurred during processor running." );
            fExceptionPtr = std::current_exception(); // capture the exception
            //fThreadRef->SetReturnException( boost::current_exception() );
            return;
        }

        LWARN( proclog, "Valid return" );

        return;
    }

} /* namespace Nymph */
