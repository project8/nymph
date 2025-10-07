/*
 * PyProcRegistrar.cc
 *
 *  Created on: Sep 24, 2025
 *      Author: N.S. Oblath
 */

#include "PyProcRegistrar.hh"
#include "ProcessorToolbox.hh"

#include "Exception.hh"

#include "logger.hh"

#include <pybind11/eval.h>

#include <memory>

//#include <sstream>

LOGGER( prlog, "PyProcRegistrar.hh" );

namespace py = pybind11;

namespace Nymph
{
    PyProcCreator::PyProcCreator( const std::string& name ) :
            Processor( name )
    {}

    PyProcCreator::~PyProcCreator()
    {}

    void PyProcCreator::Configure( const scarab::param_node& )
    {}


    PyProcRegistrar::PyProcRegistrar( const std::string& module, const std::string& type, const std::string& typeName ) :
            ProcessorRegistrar< PyProcCreator, const std::string& >( typeName ),
            fType( type ),
            fModule( module )
    {}

    PyProcRegistrar::~PyProcRegistrar() {}

    std::shared_ptr<Nymph::Processor> PyProcRegistrar::CreatePyProc( const std::string& name ) const
    {
        std::string useModule( fModule );
        if( useModule.empty() ) useModule = "__main__";
        py::object scope = py::module_::import( useModule.c_str() ).attr( "__dict__" );
        std::shared_ptr<Nymph::Processor> proc = py::eval( fType + "(\'" + name + "\')", scope ).cast< std::shared_ptr<Nymph::Processor> >();
        return proc;
    }

    std::unique_ptr<PyProcRegistrar> RegisterPyProcessor( const std::string& module, const std::string& type, const std::string& typeName )
    {
        return std::make_unique<PyProcRegistrar>( module, type, typeName );
    }

    std::shared_ptr< Nymph::Processor > CreatePyProcessor( const std::string& typeName, const std::string& name )
    {
        auto factory = ProcessorToolbox::GetProcFactory();
        //std::stringstream t_list_ss;
        //for( auto it = factory->begin(); it != factory->end(); ++it )
        //{
        //    t_list_ss << it->first << '\n';
        //}
        //LWARN( prlog, "Available processors at " << factory << ": " << t_list_ss.str() );

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
