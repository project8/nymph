/*
 * Exception.cc
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */

#include "Exception.hh"

namespace Nymph
{

    Exception::Exception() :
            boost::exception(),
            std::exception(),
            fMsgBuffer()
    {
    }
    Exception::Exception( const Exception& an_exception ) :
            boost::exception( an_exception ),
            std::exception( an_exception ),
            fMsgBuffer( an_exception.fMsgBuffer )
    {
    }

    Exception::~Exception() throw ()
    {
    }

}