/*
 * KTProcessorPybind.cc
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#ifndef NYMPH_KTPROCESSORPYBIND_HH_
#define NYMPH_KTPROCESSORPYBIND_HH_

#include "KTPyProcessor.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportKTProcessor( pybind11::module& mod )
    {
/*        pybind11::class_< KTProcessor, KTConfigurable >( mod, "KTProcessor" )
                .def( pybind11::init< const std::string & >() )

                .def( "ConnectASlot", &Nymph::KTProcessor::ConnectASlot )
                .def( "ConnectASignal", &Nymph::KTProcessor::ConnectASignal )

                .def( "GetDoBreakpoint", &Nymph::KTProcessor::GetDoBreakpoint )
                .def( "SetDoBreakpoint", &Nymph::KTProcessor::SetDoBreakpoint )
                ;
*/
        pybind11::class_< KTPyProcessor >( mod, "KTPyProcessor" )
                .def( pybind11::init< const std::string & >() )
                ;

    }

} /* namespace Nymph */

#endif /* NYMPH_KTPROCESSORPYBIND_HH_ */
