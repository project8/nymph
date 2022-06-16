/*
 * SignalSlotPybind.hh
 *
 *  Created on: Jun 15, 2022
 *      Author: N.S. Oblath
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_SIGNALSLOTBASEPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_SIGNALSLOTBASEPYBIND_HH_

#include "NymphBindingHelpers.hh"
#include "SignalSlotBase.hh"

#include "factory.hh"

#include <pybind11/cast.h>
#include <pybind11/pybind11.h>

#include <memory>

namespace py = pybind11;
using namespace py::literals;

namespace NymphPybind
{
    // SlotBase trampoline class
    class PySlotBase : public Nymph::SlotBase 
    {
        public:
            /* Inherit the constructors */
            using Nymph::SlotBase::SlotBase;

            /* Trampoline (need one for each virtual function) */
            void ConnectTo( Nymph::SignalBase* signal, int group ) override 
            {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void,            /* Return type */
                    SlotBase,        /* Parent class */
                    "connect_to",    /* Name of the function in Python */
                    ConnectTo,       /* Name of function in C++ */
                    signal, group    /* Argument(s) */
                );
            }
    };
    
    void ExportSlotBase( py::module_& nymphProcessor)
    {
        py::class_< Nymph::SlotBase, PySlotBase >( nymphProcessor, "SlotBase" )
                .def( py::init< const std::string& >() )
                .def( "connect_to", &Nymph::SlotBase::ConnectTo, "signal"_a, "group"_a=-1 )
                .def( "disconnect", &Nymph::SlotBase::Disconnect )
                .def( "disconnect_all", &Nymph::SlotBase::DisconnectAll )
                .def_property( "name", (std::string& (Nymph::SlotBase::*)()) &Nymph::SlotBase::Name,
                    [](Nymph::SlotBase& anObj, const std::string& aName ){ anObj.Name() = aName; } )
                // TODO: std::set<SignalBase*> fConnections (MEMVAR_REF_MUTABLE_CONST)
        ;
    }

    // SignalBase trampoline class
    class PySignalBase : public Nymph::SignalBase 
    {
        public:
            /* Inherit the constructors */
            using Nymph::SignalBase::SignalBase;

            /* Trampoline (need one for each virtual function) */
            void Connect( Nymph::SlotBase* slot, int group ) override 
            {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void,            /* Return type */
                    SignalBase,      /* Parent class */
                    "connect",       /* Name of the function in Python */
                    Connect,         /* Name of function in C++ */
                    slot, group      /* Argument(s) */
                );
            }
    };
    
    void ExportSignalBase( py::module_& nymphProcessor)
    {
        py::class_< Nymph::SignalBase, PySignalBase >( nymphProcessor, "SignalBase" )
                .def( py::init< const std::string& >() )
                .def( "connect", &Nymph::SignalBase::Connect, "slot"_a, "group"_a=-1 )
                .def( "disconnect", &Nymph::SignalBase::Disconnect )
                .def( "disconnect_all", &Nymph::SignalBase::DisconnectAll )
                .def_property( "name", (std::string& (Nymph::SignalBase::*)()) &Nymph::SignalBase::Name,
                    [](Nymph::SignalBase& anObj, const std::string& aName ){ anObj.Name() = aName; } )
                // TODO: std::set<SlotBase*> fConnections (MEMVAR_REF_MUTABLE_CONST)
                .def_property( "do_breakpoint", &Nymph::SignalBase::GetDoBreakpoint, &Nymph::SignalBase::SetDoBreakpoint )
        ;
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_SIGNALSLOTBASEPYBIND_HH_ */
