/*
 * PyProcRegistrar.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PYPROCREGISTRAR_HH_
#define PYTHON_BINDINGS_PROCESSOR_PYPROCREGISTRAR_HH_

#include "ProcessorRegistrar.hh"
#include "Processor.hh"

#include "Exception.hh"

#include <memory>


namespace Nymph
{
    class PyProcCreator : public Processor
    {
        public:
            PyProcCreator( const std::string& name = "test" );

            virtual ~PyProcCreator();

            void Configure( const scarab::param_node& );

    };

    class PyProcRegistrar : public ProcessorRegistrar< PyProcCreator, const std::string& >
    {
        public:
            PyProcRegistrar( const std::string& module, const std::string& type, const std::string& typeName );
            virtual ~PyProcRegistrar();

            std::shared_ptr<Nymph::Processor> CreatePyProc( const std::string& name ) const;

        protected:
            std::string fType;
            std::string fModule;
    };

    std::unique_ptr<PyProcRegistrar> RegisterPyProcessor( const std::string& module, const std::string& type, const std::string& typeName );
    std::shared_ptr< Nymph::Processor > CreatePyProcessor( const std::string& typeName, const std::string& name );

}


#endif /* PYTHON_BINDINGS_PROCESSOR_PYPROCREGISTRAR_HH_ */
