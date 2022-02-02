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
        catch(const scarab::base_exception& e)
        {
            PrintException( e, ++count );
        }
        catch(const std::exception& e)
        {
            PrintException( e, ++count );
        }
        return;
    }

    void PrintException( const std::exception& e, unsigned count )
    {
        std::string prefix = std::to_string(count) + ": ";
        LINFO( exlog, prefix << e.what() );
        try
        {
            std::rethrow_if_nested( e );
        }
        catch(const scarab::base_exception& e)
        {
            PrintException( e, ++count );
        }
        catch(const std::exception& e)
        {
            PrintException( e, ++count );
        }
        return;
    }

}
