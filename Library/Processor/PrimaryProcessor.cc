/*
 * PrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"

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

    void PrimaryProcessor::operator()( ControlAccess* control )
    {
        for( auto signalIt = fSignals.begin(); signalIt != fSignals.end(); ++signalIt )
        {
            signalIt->second->SetControl( control );
        }

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
        catch( boost::exception& e )
        {
            //fThreadRef->SetReturnException( boost::current_exception() );
        }
        return;
    }

} /* namespace Nymph */
