/*
 * KTProcessorPybind.hh
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#ifndef NYMPH_KTPROCESSORPYBIND_HH_
#define NYMPH_KTPROCESSORPYBIND_HH_

#include "KTPyProcessor.hh"
#include "KTProcessor.hh"
#include "KTSlot.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

/*    void ExportKTProcessor( pybind11::module& mod )
    {
        pybind11::class_< KTProcessor, KTConfigurable >( mod, "KTProcessor" )
                .def( pybind11::init< const std::string & >() )

                .def( "ConnectASlot", &Nymph::KTProcessor::ConnectASlot )
                .def( "ConnectASignal", &Nymph::KTProcessor::ConnectASignal )

                .def( "GetDoBreakpoint", &Nymph::KTProcessor::GetDoBreakpoint )
                .def( "SetDoBreakpoint", &Nymph::KTProcessor::SetDoBreakpoint )
                ;

        pybind11::class_< KTPyProcessor >( mod, "KTPyProcessor" )
                .def( pybind11::init< const std::string & >() )
                ;



    }*/

    // processor with wrapfunction
    class KTWrapProcessor : public KTProcessor
    {
        public:
            KTWrapProcessor( const std::string& name = "base-wrap-proc" );
            virtual ~KTWrapProcessor();

            bool Configure(const scarab::param_node& node);

            void SlotFunc(int);
            virtual void WrapFunction(int input);

        private:
            KTSlot< int > fSlot;

    };

	// helper class
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
			bool Configure(const scarab::param_node& node) override {
				PYBIND11_OVERLOAD(
						bool, // Return type
						KTWrapProcessor,      // Parent class
						Configure,          // Name of function in C++ (must match Python name)
						node      // Argument(s)
						);
			}
	};

} /* namespace Nymph */

#endif /* NYMPH_KTPROCESSORPYBIND_HH_ */
