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
                PYBIND11_OVERRIDE( /* Using PYBIND11_OVERRIDE since this is for a function with default (non-pure) implementation*/
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    WaitToContinue          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool WaitForBreakOrCanceled() override {
                PYBIND11_OVERRIDE(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    WaitForBreakOrCanceled          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void WaitForEndOfRun() override {
                PYBIND11_OVERRIDE(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    WaitForEndOfRun          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Continue() override {
                PYBIND11_OVERRIDE(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    Continue          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Cancel( int code = 0 ) override {
                PYBIND11_OVERRIDE(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    Cancel          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool IsCanceled() const override {
                PYBIND11_OVERRIDE(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    IsCanceled          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void Break() override {
                PYBIND11_OVERRIDE(
                    void, /* Return type */
                    Controller,      /* Parent class */
                    Break          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            bool IsAtBreak() const override {
                PYBIND11_OVERRIDE(
                    bool, /* Return type */
                    Controller,      /* Parent class */
                    IsAtBreak          /* Name of function in C++ (must match Python name) */
                );
            }

             /* Trampoline */
            void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() ) override {
                PYBIND11_OVERRIDE(
                    void, /* Return type */
                    Controller,      /* Parent class */
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
                .def("WaitToContinue", &Nymph::Controller::WaitToContinue)
                .def("WaitForBreakOrCanceled", &Nymph::Controller::WaitForBreakOrCanceled)
                .def("WaitForEndOfRun", &Nymph::Controller::WaitForEndOfRun)
                .def("Continue", &Nymph::Controller::Continue)
                .def("Cancel", &Nymph::Controller::Cancel, py::arg("code") = 0 )
                .def("IsCanceled", &Nymph::Controller::IsCanceled)
                .def("Break", &Nymph::Controller::Break)
                .def("IsAtBreak", &Nymph::Controller::IsAtBreak)
                .def("ChainIsQuitting", &Nymph::Controller::ChainIsQuitting, py::arg("name"), py::arg("ePtr") = std::exception_ptr() );
    }

}

#endif /* PYTHON_BINDINGS_PROCESSOR_PROCESSORPYBIND_HH_ */
