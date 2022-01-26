/*
 * Exception.cc
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */

#include "Exception.hh"

#include "logger.hh"

LOGGER( exlog, "Exception" );

namespace Nymph
{
    void PrintException( const scarab::base_exception& e, unsigned count )
    {
        std::string prefix = std::to_string(count) + ": ";
        LINFO( exlog, prefix << "Thrown at: " << e.where() );
        LINFO( exlog, prefix << e.what() );
        try
        {
            std::rethrow_if_nested( e );
        }
        catch(const Nymph::Exception& e)
        {
            PrintException( e, ++count );
        }
        return;
    }

/*
    BaseException::BaseException() :
            std::exception(),
            fAtFilename( "unknown" ),
            fAtLineNumber( 0 ),
            fBuffer()
    {}

    BaseException::~BaseException() noexcept
    {}

    const char* BaseException::where() const noexcept
    {
        try
        {
            fBuffer = fAtFilename + "(" + std::to_string(fAtLineNumber) + ")";
        }
        catch(const std::bad_alloc& e)
        {
            fBuffer = fAtFilename;
        }
        return fBuffer.c_str();
    }
*/
/* removed for trying home-grown exceptions, 1/10/22
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
*/
}
