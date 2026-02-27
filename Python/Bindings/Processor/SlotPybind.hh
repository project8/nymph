/*
 * SlotPybind.hh
 *
 *  Created on: Jun 15, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <memory>

#include "SignalSlotBase.hh"
#include "Processor.hh"

namespace py = pybind11;

namespace NymphPybind
{
    
    //trampoline class
    class PySlotBase : public Nymph::SlotBase {
        public:
            /* Inherit the constructors */
            using Nymph::SlotBase::SlotBase;

            /* Trampoline (need one for each virtual function) */
            void ConnectTo( Nymph::SignalBase* signal, int group = -1 ) override {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void, /* Return type */
                    Nymph::SlotBase,      /* Parent class */
                    "connect_to",       /* Name of function in python */
                    ConnectTo,          /* Name of function in C++ */
                    signal,
                    group/* Argument(s) */
                );
            }
            
            /* Trampoline (need one for each virtual function) */
            bool MatchesTo( Nymph::SignalBase* signal ) override {
                PYBIND11_OVERRIDE_PURE_NAME(
                    bool, /* Return type */
                    Nymph::SlotBase,      /* Parent class */
                    "matches_to",  /* Name of function in python */
                    MatchesTo,          /* Name of function in C++ */
                    signal/* Argument(s) */
                );
            }
    };
    
    void ExportSlotBase( py::module_& nymphProcessor)
    {

       py::class_< Nymph::SlotBase, PySlotBase, std::shared_ptr<Nymph::SlotBase > >(nymphProcessor, "_SlotBase")
                .def(py::init<const std::string& >())
                .def(py::init<const std::string&, Nymph::Processor* >())
                
                .def("connect_to", &Nymph::SlotBase::ConnectTo)
                .def("matches_to", &Nymph::SlotBase::MatchesTo)
                .def("disconnect", &Nymph::SlotBase::Disconnect)
                .def("disconnect_all", &Nymph::SlotBase::DisconnectAll)
                .def_property("connections", static_cast< std::set< Nymph::SignalBase* >& (Nymph::SlotBase::*)() const>(&Nymph::SlotBase::Connections),
                                            [](Nymph::SlotBase& slot, const std::set< Nymph::SignalBase* >& signals){slot.Connections() = signals;} )
                .def_property("name", static_cast< const std::string& (Nymph::SlotBase::*)() const>(&Nymph::SlotBase::Name),
                                      [](Nymph::SlotBase& slot, const std::string& name){slot.Name() = name;} );
                
        
    }
    
    
    template< typename... XArgs >
    void ExportSlot( py::module_& nymphProcessor, const std::string& typestr)
    {
        std::string pyClsName = std::string("_Slot") + typestr;
        
        using signature = void( XArgs... );

        py::class_< Nymph::Slot< XArgs... >, Nymph::SlotBase, std::shared_ptr<Nymph::Slot< XArgs... > > >(nymphProcessor, pyClsName.c_str())
                .def(py::init<const std::string&, const std::function< signature >& >())
                .def(py::init<const std::string&,  Nymph::Processor*, const std::function< signature >& >())
                .def("__call__", &Nymph::Slot< XArgs... >::operator())
                .def_property("function", static_cast< const std::function< signature >& (Nymph::Slot< XArgs... >::*)() const>(&Nymph::Slot< XArgs... >::Function),
                                      [](Nymph::Slot< XArgs... >& slot, const std::function< signature >& function){slot.Function() = function;} );
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_ */
