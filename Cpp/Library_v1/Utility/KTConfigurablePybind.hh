/*
 * KTConfigurablePybind.cc
 *
 *  Created on: Jan 27, 2018
 *      Author: N. Oblath
 */

#include "KTConfigurable.hh"

#include "pybind11/pybind11.h"

namespace Nymph
{

    void ExportKTConfigurable( pybind11::module& mod )
    {
/*        pybind11::class_< KTConfigurable >( mod, "KTConfigurable" )
                .def( pybind11::init< const std::string & >() )

                .def( "Configure", (bool (KTConfigurable::*)(const std::string&)) &KTConfigurable::Configure )
                //.def( "Configure", (void (KTConfigurable::*)(const scarab::param_node&)) &KTConfigurable::Configure )

                .def( "GetConfigName", &KTConfigurable::GetConfigName )
                .def( "SetConfigName", &KTConfigurable::SetConfigName )
                ;
*/
    }

} /* namespace Nymph */
