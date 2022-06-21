/*
 * ControllerPybind.hh
 *
 *  Created on: June 15, 2022
 *      Author: P. T. Surukuchi
 */

#ifndef PYTHON_BINDINGS_PROCESSOR_CONTROLLERPYBIND_HH_
#define PYTHON_BINDINGS_PROCESSOR_CONTROLLERPYBIND_HH_

#include <pybind11/pybind11.h>
#include <memory>

#include "Controller.hh"
#include "factory.hh"

namespace py = pybind11;

namespace NymphPybind
{
    //trampoline class, see https://pybind11.readthedocs.io/en/stable/advanced/classes.html
    class PyController : public Nymph::Controller {
        public:
            /* Inherit the constructors */
            using Nymph::Controller::Controller;

            /* Trampoline (need one for each virtual function) */
            bool WaitToContinue() override {
                PYBIND11_OVERRIDE_NAME( /* Using PYBIND11_OVERRIDE since this is for a function with default (non-pure) implementation*/
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    "wait_to_continue",   // Name of method in Python (name)
                    WaitToContinue          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool WaitForBreakOrCanceled() override {
                PYBIND11_OVERRIDE_NAME(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    "wait_for_break_or_canceled",   // Name of method in Python (name)
                    WaitForBreakOrCanceled          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void WaitForEndOfRun() override {
                PYBIND11_OVERRIDE_NAME(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    "wait_for_end_of_run",   // Name of method in Python (name)
                    WaitForEndOfRun          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Continue() override {
                PYBIND11_OVERRIDE_NAME(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    "continue",   // Name of method in Python (name)
                    Continue          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Cancel( int code = 0 ) override {
                PYBIND11_OVERRIDE_PURE_NAME(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    "cancel",   // Name of method in Python (name)
                    Cancel          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool IsCanceled() const override {
                PYBIND11_OVERRIDE_NAME(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    "is_canceled",   // Name of method in Python (name)
                    IsCanceled          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Break() override {
                PYBIND11_OVERRIDE_NAME(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    "break",   // Name of method in Python (name)
                    Break          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool IsAtBreak() const override {
                PYBIND11_OVERRIDE_NAME(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    "is_at_break",   // Name of method in Python (name)
                    IsAtBreak          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() ) override {
                PYBIND11_OVERRIDE_NAME(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    "chain_is_quitting",   // Name of method in Python (name)
                    ChainIsQuitting,          /* Name of function in C++ (must match Python name) */
                    name,
                    ePtr
                );
            }
    };

    // std::shared_ptr< Nymph::Controller > get_controller(const std::string& type, const std::string& name)
    // {
    //     scarab::factory< Nymph::Controller, const std::string& >* factory = scarab::factory< Nymph::Controller, const std::string& >::get_instance();
    //     return std::shared_ptr< Nymph::Controller >(factory->create(type, name));
    // }

    void ExportController( py::module_& nymphController)
    {
        py::class_< Nymph::Controller, PyController, std::shared_ptr<Nymph::Controller> >(nymphController, "_Controller")
                .def(py::init< >())
                .def("wait_to_continue", &Nymph::Controller::WaitToContinue)
                .def("wait_for_break_or_canceled", &Nymph::Controller::WaitForBreakOrCanceled)
                .def("wait_for_end_of_run", &Nymph::Controller::WaitForEndOfRun)
                .def("continue", &Nymph::Controller::Continue)
                .def("cancel", &Nymph::Controller::Cancel, py::arg("code") = 0 )
                .def("is_canceled", &Nymph::Controller::IsCanceled)
                .def("break", &Nymph::Controller::Break)
                .def("is_at_break", &Nymph::Controller::IsAtBreak)
                .def("chain_is_quitting", &Nymph::Controller::ChainIsQuitting, py::arg("name"), py::arg("ePtr") = std::exception_ptr() );
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_ */
