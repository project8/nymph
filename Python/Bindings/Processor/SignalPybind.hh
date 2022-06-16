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

#include "NymphBindingHelpers.hh"
#include "SignalSlotBase.hh"
#include "Signal.hh"
#include "Slot.hh"
#include "Processor.hh"

namespace py = pybind11;

namespace NymphPybind
{
    
    //trampoline class
    class PySignalBase : public Nymph::SignalBase {
        public:
            /* Inherit the constructors */
            using Nymph::SignalBase::SignalBase;

            /* Trampoline (need one for each virtual function) */
            void Connect( Nymph::SlotBase* slot, int group = -1 ) override {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void, /* Return type */
                    Nymph::SignalBase,      /* Parent class */
                    connect,            /* Name of function in python */
                    Connect,          /* Name of function in C++ */
                    slot,
                    group/* Argument(s) */
                );
            }
    };
    
    void ExportSignalBase( py::module_& nymphProcessor)
    {

       py::class_< Nymph::SignalBase, PySignalBase, std::shared_ptr<Nymph::SignalBase > >(nymphProcessor, "_SignalBase")
                .def(py::init<const std::string& >())
                .def("connect", &Nymph::SignalBase::Connect, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("disconnect", &Nymph::SignalBase::Disconnect)
                .def("disconnect_all", &Nymph::SignalBase::DisconnectAll)
                .def_property_readonly("connections", static_cast< std::set< Nymph::SlotBase* >& (Nymph::SignalBase::*)() const>(&Nymph::SignalBase::Connections))
                .def_property("name", static_cast< const std::string& (Nymph::SignalBase::*)() const>(&Nymph::SignalBase::Name),
                                      [](Nymph::SignalBase& signal, const std::string& name){signal.Name() = name;} )
                .def_property("do_breakpoint", &Nymph::SignalBase::GetDoBreakpoint, &Nymph::SignalBase::SetDoBreakpoint);
        
    }
    
    template< typename... XArgs >
    void ExportSignal( py::module_& nymphProcessor, const std::string& typestr)
    {
        std::string pyClsName = std::string("_Signal") + typestr;

       py::class_< Nymph::Signal< XArgs... >, Nymph::SignalBase, std::shared_ptr<Nymph::Signal< XArgs... > > >(nymphProcessor, pyClsName.c_str())
                .def(py::init<const std::string& >())
                .def(py::init<const std::string&,  Nymph::Processor* >())
                .def("emit", &Nymph::Signal< XArgs... >::Emit, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("__call__", &Nymph::Signal< XArgs... >::operator(), NYMPH_BIND_CALL_GUARD_STREAMS);
        
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_ */
