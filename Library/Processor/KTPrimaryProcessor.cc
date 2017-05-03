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

    KTPrimaryProcessor::KTPrimaryProcessor( const std::string& name ) :
            KTProcessor( name ),
            fThreadRef()
    {
    }

    KTPrimaryProcessor::~KTPrimaryProcessor()
    {
    }

    void KTPrimaryProcessor::operator ()()
    {
        if( ! Run() )
        {
            KTERROR( proclog, "An error occurred during processor running." );
            THROW_RETURN_EXCEPTION( fThreadRef.fDataPtrRet, KTException() << "An error occurred during processor running" );
        }
        else
        {
            fThreadRef.fDataPtrRet.set_value( KTDataPtr() );
        }
        return;
    }

} /* namespace Nymph */
