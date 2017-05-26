/*
 * KTException.cc
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */

#include "KTException.hh"

namespace Nymph
{

    KTException::KTException() :
            boost::exception(),
            std::exception(),
            fMsgBuffer()
    {
    }
    KTException::KTException( const KTException& an_exception ) :
            boost::exception( an_exception ),
            std::exception( an_exception ),
            fMsgBuffer( an_exception.fMsgBuffer )
    {
    }

    KTException::~KTException() throw ()
    {
    }

}
