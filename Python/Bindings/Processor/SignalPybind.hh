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

#include "SignalSlotBase.hh"
#include "Signal.hh"
#include "Slot.hh"
#include "Processor.hh"

namespace py = pybind11;

namespace NymphPybind
{
    
    template< typename... XArgs >
    void explicit_name_setter(Nymph::Signal< XArgs... >& signal)
    {
        
    }

    template< typename... XArgs >
    void ExportSignal( py::module_& nymphProcessor, const std::string& typestr)
    {
        std::string pyClsName = std::string("_Signal") + typestr;
        
       // py::class_< Nymph::Signal< XArgs... >, std::shared_ptr<Nymph::Signal< XArgs... > > >(nymphProcessor, pyClsName.c_str())
       py::class_< Nymph::Signal< XArgs... >, std::shared_ptr<Nymph::Signal< XArgs... > > >(nymphProcessor, pyClsName.c_str())
                .def(py::init<const std::string& >())
                .def(py::init<const std::string&,  Nymph::Processor* >())
                .def("emit", &Nymph::Signal< XArgs... >::Emit)
                .def("__call__", &Nymph::Signal< XArgs... >::operator())
                .def("connect", &Nymph::Signal< XArgs...>::Connect)
                .def("disconnect", &Nymph::Signal< XArgs... >::Disconnect)
                .def("disconnect_all", &Nymph::Signal< XArgs... >::DisconnectAll)
               // .def_property_readonly("connections", static_cast<const std::set< Nymph::SlotBase* >& (Nymph::Signal< XArgs... >::*)() const>(&Nymph::Signal< XArgs... >::Connections))
                //.def_property("name", static_cast< const std::string& (Nymph::Signal< XArgs... >::*)() const>(&Nymph::Signal< XArgs... >::Name),
                //                      static_cast< const std::string& (Nymph::Signal< XArgs... >::*)() const>(&Nymph::Signal< XArgs... >::Name) )
                .def_property("do_breakpoint", &Nymph::Signal< XArgs... >::GetDoBreakpoint, &Nymph::Signal< XArgs... >::SetDoBreakpoint);
        
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_SIGNALPYBIND_HH_ */
