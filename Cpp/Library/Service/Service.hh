/**
 @file Service.hh
 @brief Base class for processors
 @author: N. S. Oblath
 @date: Dec 28, 2023
 */

#ifndef NYMPH_SERVICE_HH_
#define NYMPH_SERVICE_HH_

#include "Exception.hh"
#include "MemberVariable.hh"

#include "factory.hh"
#include "param.hh"

#include <map>

namespace Nymph
{
    struct ServiceException : virtual public Exception {};

    class Service
    {
        public:
            Service( const std::string& name );
            virtual ~Service();

            template< class XDerivedProc >
            static scarab::registrar< Nymph::Service, XDerivedProc, const std::string& >* RegisterService( const std::string& name );

        public:
            /// Configure the processor with a param_node
            virtual void Configure( const scarab::param_node& node ) = 0;

            MEMVAR_REF( std::string, Name );
    };


    template< class XDerivedProc >
    scarab::registrar< Service, XDerivedProc, const std::string& >* Service::RegisterService( const std::string& name )
    {
        return new scarab::registrar< Service, XDerivedProc, const std::string& >( name );
    }

#define REGISTER_SERVICE_NONAMESPACE(proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::Service, proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

#define REGISTER_SERVICE_NAMESPACE(proc_namespace, proc_class, proc_name) \
        static ::scarab::registrar< ::Nymph::Service, ::proc_namespace::proc_class, const std::string& > sProc##proc_class##Registrar( proc_name );

// Macro overloading trick from here: https://stackoverflow.com/a/11763277
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
/// Services defined in a namespace need to specify the namespace first:
///   [no namespace]: REGISTER_PROCESSOR( [class], [name in quotes] )
///   [with namespace]: REGISTER_PROCESSOR( [namespace], [class], [name in quotes] )
#define REGISTER_SERVICE(...) GET_MACRO(__VA_ARGS__, REGISTER_SERVICE_NAMESPACE, REGISTER_SERVICE_NONAMESPACE, )(__VA_ARGS__)

} /* namespace Nymph */

#endif /* NYMPH_SERVICE_HH_ */
