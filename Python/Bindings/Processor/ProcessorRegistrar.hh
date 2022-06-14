/*
 * ProcessorRegistrar.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_
#define PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_

#include "Processor.hh"

#include "logger.hh"

#include <pybind11/eval.h>

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

            Processor* CreatePythonProcessor( const std::string& moduleName, const std::string& typeName )
            {
                /*
                py::object scope = py::module_::import("__main__").attr("__dict__");
                std::string execStr = std::string("module = __import__(") + moduleName + ")\n";
                execStr += "class_ = getattr(module, " + typeName + ")";
                py::exec( execStr.c_str() );
                Processor* proc = py::eval("instance = class_()", scope).cast<Processor*>();
                */
                py::object scope = py::module_::import( fModuleName ).attr( "__dict__" );
                Processor* proc = py::eval( fTypeName + "(" + fName + ")", scope ).cast<Processor*>();
                LWARN( prlog_h, "Processor is at: " << proc );
                return proc;
            }

    };

    class PyProcRegistrar : public scarab::registrar< Processor, PyProcCreator, const std::string& >
    {
        public:
            PyProcRegistrar( const std::string& moduleName, const std::string& typeName, const std::string& name ) :
                    scarab::registrar< Processor, PyProcCreator, const std::string& >( name ),
                    fTypeName( typeName ),
                    fModuleName( moduleName )
            {}
            virtual ~PyProcRegistrar() {}

            Processor* create( const std::string& name ) const
            {
                PyProcCreator ppc( name );
                return ppc.CreatePythonProcessor( fModuleName, fTypeName );
            }

        protected:
            std::string fTypeName;
            std::string fModuleName;
    }

    PyProcRegistrar* RegisterPyProcessor( const std::string& moduleName, const std::string& typeName, const std::string& name )
    {
        return new PyProcRegistrar( typeName, name );
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYREGISTRAR_HH_ */
