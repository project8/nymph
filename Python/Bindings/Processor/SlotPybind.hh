/*
 * SlotPybind.hh
 *
 *  Created on: Jun 15, 2022
 *      Author: F. Thomas
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_

#include <pybind11/pybind11.h>
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
                PYBIND11_OVERRIDE_PURE(
                    void, /* Return type */
                    SlotBase,      /* Parent class */
                    ConnectTo,          /* Name of function in C++ (must match Python name) */
                    signal,
                    group/* Argument(s) */
                );
            }
            
            /* Trampoline (need one for each virtual function) */
            bool MatchesTo( Nymph::SignalBase* signal ) override {
                PYBIND11_OVERRIDE_PURE(
                    bool, /* Return type */
                    SlotBase,      /* Parent class */
                    MatchesTo,          /* Name of function in C++ (must match Python name) */
                    signal/* Argument(s) */
                );
            }
    };
    
    void ExportSlot( py::module_& nymphProcessor)
    {

       py::class_< Nymph::SlotBase, PySlotBase, std::shared_ptr<Nymph::SlotBase > >(nymphProcessor, "_Slot")
                .def(py::init<const std::string& >())
                //.def(py::init<const std::string&,  Nymph::Processor* >())
                .def("connect_to", &Nymph::SlotBase::ConnectTo)
                .def("MatchesTo", &Nymph::SlotBase::MatchesTo)
                .def("disconnect", &Nymph::SlotBase::Disconnect)
                .def("disconnect_all", &Nymph::SlotBase::DisconnectAll)
                .def_property("connections", static_cast< std::set< Nymph::SignalBase* >& (Nymph::SlotBase::*)() const>(&Nymph::SlotBase::Connections),
                                            [](Nymph::SlotBase& slot, const std::set< Nymph::SignalBase* >& signals){slot.Connections() = signals;} )
                .def_property("name", static_cast< const std::string& (Nymph::SlotBase::*)() const>(&Nymph::SlotBase::Name),
                                      [](Nymph::SlotBase& slot, const std::string& name){slot.Name() = name;} );
                
                
                        //~ public:

            //~ virtual void ConnectTo( SignalBase* signal, int group = -1 ) = 0;

            //~ void Disconnect( SignalBase* signal);
            //~ void DisconnectAll();

            //~ typedef std::set< SignalBase* > SignalConnections; // to get around the problem of having a comma inside a macro function argument
            //~ MEMVAR_REF_MUTABLE( SignalConnections, Connections );


        //~ protected:
            //~ friend class SignalBase;
            //~ virtual void AddConnection( SignalBase* signal );
        
    }

}


#endif /* PYTHON_BINDINGS_PROCESSOR_SLOTPYBIND_HH_ */
