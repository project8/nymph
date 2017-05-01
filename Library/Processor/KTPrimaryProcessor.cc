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
    KTLOGGER(proclog, "KTPrimaryProcessor");

    KTPrimaryProcessor::KTPrimaryProcessor(const std::string& name) :
            KTProcessor(name)
    {
    }

    KTPrimaryProcessor::~KTPrimaryProcessor()
    {
    }

    void KTPrimaryProcessor::operator ()( KTDataPtrReturn ret )
    {
        if (! Run( ret ))
        {
            KTERROR(proclog, "An error occurred during processor running.");
            THROW_RETURN_EXCEPTION( ret, KTException() << "An error occurred during processor running" );
        }
        else
        {
            ret.set_value( KTDataPtr() );
        }
        return;
    }

} /* namespace Nymph */
