/*
 * SignalPybind.hh
 *
 *  Created on: Jun 13, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "Signal.hh"
#include "Processor.hh"

namespace py = pybind11;

namespace NymphPybind
{
    //~ //trampoline class
    //~ class PyProcessor : public Nymph::Processor {
        //~ public:
            //~ /* Inherit the constructors */
            //~ using Nymph::Processor::Processor;

            //~ /* Trampoline (need one for each virtual function) */
            //~ void Configure( const scarab::param_node& node ) override {
                //~ PYBIND11_OVERRIDE_PURE(
                    //~ void, /* Return type */
                    //~ Processor,      /* Parent class */
                    //~ Configure,          /* Name of function in C++ (must match Python name) */
                    //~ node/* Argument(s) */
                //~ );
            //~ }
    //~ };

    template< typename... XArgs >
    void ExportSignal( py::module_& nymphProcessor, const std::string& typestr)
    {
        std::string pyClsName = std::string("_Signal") + typestr;
        
        py::class_< Nymph::Signal< XArgs... >, std::shared_ptr<Nymph::Signal< XArgs... > > >(nymphProcessor, pyClsName.c_str())
                .def(py::init<const std::string& >())
                .def(py::init<const std::string&,  Nymph::Processor* >())
                .def("emit", &Nymph::Signal< XArgs... >::Emit)
                .def("__call__", &Nymph::Signal< XArgs... >::operator());
        
    }

}

            //~ template< typename XOwner >
            //~ Signal( const std::string& name, XOwner* owner );


            //~ virtual void Connect( SlotBase* slot, int group = -1 );

#endif /* PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_ */
