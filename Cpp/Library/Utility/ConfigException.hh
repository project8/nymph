/**
 @file ConfigException.hh
 @brief Exceptions related to configurations
 @author: N. S. Oblath
 @date: Jan 5, 2012
 */

#ifndef NYMPH_CONFIG_EXCEPTION_HH_
#define NYMPH_CONFIG_EXCEPTION_HH_

#include "Exception.hh"

#include "MemberVariable.hh"

#include "param.hh"

namespace Nymph
{
    /*!
     @class ConfigException
     @author N. S. Oblath

     @brief An exception class for use with errors that occur during configuration.

     @details

     The param_node of the configuration in question can be attached to the exception using the constructor 
     or the function Config().  

    */
    class ConfigException : public scarab::typed_exception< ConfigException >
    {
        public:
            ConfigException() noexcept;
            ConfigException( const std::string& a_filename, int a_line ) noexcept;
            ConfigException( const scarab::param& config ) noexcept;
            ConfigException( const ConfigException& orig ) noexcept;
            virtual ~ConfigException() noexcept;

            virtual const char* what() const noexcept;

            MEMVAR_REF( scarab::param_ptr_t, Config );

        protected:
            mutable std::string fConfigStr;
    };

} /* namespace Nymph */

#endif /* NYMPH_CONFIG_EXCEPTION_HH_ */
