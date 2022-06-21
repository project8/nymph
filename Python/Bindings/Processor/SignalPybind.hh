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
    
    template< typename... XArgs >
    void ExportSignal( py::module_& nymphProcessor, const std::string& typestr)
    {
        std::string pyClsName = std::string("_Signal") + typestr;

       py::class_< Nymph::Signal< XArgs... >, std::shared_ptr<Nymph::Signal< XArgs... > > >(nymphProcessor, pyClsName.c_str())
                .def(py::init<const std::string& >())
                .def(py::init<const std::string&,  Nymph::Processor* >())
                .def("emit", &Nymph::Signal< XArgs... >::Emit, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("__call__", &Nymph::Signal< XArgs... >::operator(), NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("connect", &Nymph::Signal< XArgs...>::Connect, NYMPH_BIND_CALL_GUARD_STREAMS)
                .def("disconnect", &Nymph::Signal< XArgs... >::Disconnect)
                .def("disconnect_all", &Nymph::Signal< XArgs... >::DisconnectAll)
                MEMVAR_REF_MUTABLE_CONST_PY("connections", Connections, std::set< Nymph::SlotBase* >, Nymph::Signal< XArgs... >)
                // .def_property_readonly("connections", static_cast< std::set< Nymph::SlotBase* >& (Nymph::Signal< XArgs... >::*)() const>(&Nymph::Signal< XArgs... >::Connections))
                MEMVAR_REF_PY("name", Name, std::string, Nymph::Signal< XArgs... >)
                // .def_property("name", static_cast< const std::string& (Nymph::Signal< XArgs... >::*)() const>(&Nymph::Signal< XArgs... >::Name),
                                    //   [](Nymph::Signal< XArgs... >& aName, const std::string& name){aName.Name() = name;} )
                MEMVAR_PY("do_breakpoint", DoBreakpoint, Nymph::Signal< XArgs... >);
                // .def_PROPERTY("do_breakpoint", Nymph::SignalBase::GetDoBreakpoint, Nymph::SignalBase::SetDoBreakpoint);
        // MEMVAR(bool, DoBreakpoint)
        // MEMVAR_Py(DoBreakpoint, ) 
        
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_ */
