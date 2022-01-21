/*
 * PrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"

//#include "ControlAccess.hh"
#include "QuitChain.hh"

#include "logger.hh"

namespace Nymph
{
    LOGGER( proclog, "PrimaryProcessor" );

    PrimaryProcessor::PrimaryProcessor( const std::string& name ) :
            Processor( name )//,
//            fExceptionPtr()
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
        catch( const QuitChain& e )
        {
            LINFO( proclog, "Processor chain started by <" << fName << "> is quitting" );
            fExceptionPtr = std::current_exception(); // capture the exception
            ControlAccess::get_instance()->ChainQuitting( fName, fExceptionPtr );
            return;
        }
        catch( const std::exception& e )
        {
            LERROR( proclog, "An error occurred during processor running: " << e.what() );
            fExceptionPtr = std::current_exception(); // capture the exception
            ControlAccess::get_instance()->ChainQuitting( fName, fExceptionPtr );
            return;
        }

        LWARN( proclog, "Valid return" );
        ControlAccess::get_instance()->ChainQuitting( fName );

        return;
    }

} /* namespace Nymph */
