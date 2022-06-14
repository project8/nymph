/*
 * ProcessorRegistrar.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_

#include "Processor.hh"

#include "Exception.hh"

#include "logger.hh"

#include <pybind11/eval.h>

#include <memory>

LOGGER( prlog_h, "ProcessorRegistrar.hh" );

namespace Nymph
{
    class PyProcCreator : public Processor
    {
        public:
            PyProcCreator( const std::string& name = "test" ) :
                    Processor( name )
            {}

            virtual ~PyProcCreator()
            {}

            void Configure( const scarab::param_node& )
            {}

    };

    class PyProcRegistrar : public scarab::registrar< Processor, PyProcCreator, const std::string& >
    {
        public:
            PyProcRegistrar( const std::string& module, const std::string& type, const std::string& typeName ) :
                    scarab::registrar< Processor, PyProcCreator, const std::string& >( typeName ),
                    fType( type ),
                    fModule( module )
            {}
            virtual ~PyProcRegistrar() {}

            std::shared_ptr<Nymph::Processor> CreatePyProc( const std::string& name ) const
            {
                std::string useModule( fModule );
                if( useModule.empty() ) useModule = "__main__";
                py::object scope = py::module_::import( useModule.c_str() ).attr( "__dict__" );
                std::shared_ptr<Nymph::Processor> proc = py::eval( fType + "(\'" + name + "\')", scope ).cast< std::shared_ptr<Nymph::Processor> >();
                LWARN( prlog_h, "Processor is at: " << proc.get() );
                return proc;

            }

        protected:
            std::string fType;
            std::string fModule;
    };

    std::unique_ptr<PyProcRegistrar> RegisterPyProcessor( const std::string& module, const std::string& type, const std::string& typeName )
    {
        return std::unique_ptr<PyProcRegistrar>( new PyProcRegistrar( module, type, typeName ) );
    }

    std::shared_ptr< Nymph::Processor > CreatePyProcessor( const std::string& typeName, const std::string& name )
    {
        scarab::factory< Nymph::Processor, const std::string& >* factory = scarab::factory< Nymph::Processor, const std::string& >::get_instance();
        if( ! factory->has_class( typeName ) )
        {
            THROW_EXCEPT_HERE( Nymph::Exception() << "Did not find processor with type <" << typeName << ">" );
        }
        const PyProcRegistrar* ppReg = dynamic_cast< const PyProcRegistrar* >( factory->get_registrar( typeName ) );
        if( ! ppReg )
        {
            THROW_EXCEPT_HERE( Nymph::Exception() << "Registrar did not cast correctly for <" << typeName << ">" );
        }
        return ppReg->CreatePyProc( name );
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_ */
