/*
 * ConfigException.cc
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#include "ConfigException.hh"

namespace Nymph
{
    //KTLOGGER(proclog, "Processor");

    ConfigException::ConfigException() noexcept :
            scarab::typed_exception< ConfigException >(),
            fConfig(),
            fConfigStr()
    {}

    ConfigException::ConfigException( const std::string& a_filename, int a_line ) noexcept :
            scarab::typed_exception< ConfigException >( a_filename, a_line ),
            fConfig(),
            fConfigStr()
    {}

    ConfigException::ConfigException( const scarab::param& config ) noexcept :
            scarab::typed_exception< ConfigException >(),
            fConfig(),
            fConfigStr()
    {
        try
        {
            fConfig = config.clone();
        }
        catch( ... )
        {}
    }

    ConfigException::ConfigException( const ConfigException& orig ) noexcept :
            scarab::typed_exception< ConfigException >( orig ),
            fConfig(),
            fConfigStr()
    {
        try
        {
            if( orig.fConfig ) fConfig = orig.fConfig->clone();
        }
        catch( ... )
        {}
        
    }

    ConfigException::~ConfigException() noexcept
    {}

    const char* ConfigException::what() const noexcept
    {
        try
        {
            fConfigStr = f_what + '\n' + fConfig->to_string();
            return fConfigStr.c_str();
        }
        catch( ... )
        {
            return f_what.c_str();
        }
    }

} /* namespace Nymph */
