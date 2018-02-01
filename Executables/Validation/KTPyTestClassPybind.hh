/*
 * KTPyTestClassPybind.cc
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#include "KTPyTestClass.hh"
#include "KTTestProcessor.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportKTPyTestClass( pybind11::module& mod )
    {
        pybind11::class_< Nymph::KTPyTestClass >( mod, "KTPyTestClass" )
                .def( pybind11::init<>() )
                .def( "SayHello", &Nymph::KTPyTestClass::SayHello )
                .def( "GetValue", &Nymph::KTPyTestClass::GetValue )
                .def( "SetValue", &Nymph::KTPyTestClass::SetValue )
                ;
    };

	class KTPyWrapProcessor: public KTWrapProcessor
	{
		public:
			// Inherit the constructors
			using KTWrapProcessor::KTWrapProcessor;

			// Trampoline (need one for each virtual function)
			void WrapFunction(int input) override {
				PYBIND11_OVERLOAD_PURE(
						void, // Return type
						KTWrapProcessor,      // Parent class
						WrapFunction,          // Name of function in C++ (must match Python name)
						input      // Argument(s)
						);
				}
	};


} /* namespace Nymph */
