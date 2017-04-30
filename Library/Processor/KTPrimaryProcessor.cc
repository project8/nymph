/*
 * KTPrimaryProcessor.cc
 *
 *  Created on: Oct 10, 2012
 *      Author: nsoblath
 */

#include "KTPrimaryProcessor.hh"

#include "KTLogger.hh"

namespace Nymph
{
    KTLOGGER(proclog, "KTPrimaryProcessor");

    KTPrimaryProcessor::KTPrimaryProcessor(const std::string& name) :
            KTProcessor(name)
    {
    }

    KTPrimaryProcessor::~KTPrimaryProcessor()
    {
    }

    void KTPrimaryProcessor::operator ()( std::promise< KTDataPtr >& promise )
    {
        if (! Run( promise ))
        {
            KTERROR(proclog, "An error occurred during processor running.");
            //TODO: make sure we're using the exception pointer correctly
            promise.set_exception( std::make_exception_ptr( std::exception( "An error occurred during processor running" ) ) );
        }
        else
        {
            promise.set_value( KTDataPtr() );
        }
        return;
    }

} /* namespace Nymph */
