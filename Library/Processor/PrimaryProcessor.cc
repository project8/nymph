/*
 * PrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"

#include "ControlAccess.hh"
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
            ControlAccess::get_instance()->ChainIsQuitting( fName, std::current_exception() );
            return;
        }
        catch( const std::exception& e )
        {
            LERROR( proclog, "An error occurred during processor running: " << e.what() );
            ControlAccess::get_instance()->ChainIsQuitting( fName, std::current_exception() );
            return;
        }

        LWARN( proclog, "Valid return" );
        ControlAccess::get_instance()->ChainIsQuitting( fName );

        return;
    }

} /* namespace Nymph */
