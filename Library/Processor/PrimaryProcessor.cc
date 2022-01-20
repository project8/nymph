/*
 * PrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"

//#include "ControlAccess.hh"
#include "QuitThread.hh"

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

    void PrimaryProcessor::operator()()
    {
        // go!
        try
        {
            Run();
        }
        catch( const QuitThread& e )
        {
            LINFO( proclog, "Processor thread started by <" << fName << "> is quitting" );
            fExceptionPtr = std::current_exception(); // capture the exception
            /////SharedControl::get_instance()->DecrementActiveThreads();
            // we don't initiate cancellation on QuitThread; this type of quitting has been initiated elsewhere and detected in this thread.
            return;
        }
        catch( const std::exception& e )
        {
            LERROR( proclog, "An error occurred during processor running: " << e.what() );
            fExceptionPtr = std::current_exception(); // capture the exception
            /////SharedControl* control = SharedControl::get_instance();
            /////control->DecrementActiveThreads();
            /////control->Cancel();
            return;
        }

        /////SharedControl::get_instance()->DecrementActiveThreads();
        LWARN( proclog, "Valid return" );

        return;
    }

} /* namespace Nymph */
