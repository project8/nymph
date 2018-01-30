/*
 * NymphPybind.cc
 *
 *  Created on: Jan 27, 2018
 *      Author: obla999
 */

#include "Application/KTProcessorToolboxPybind.hh"
#include "Processor/KTProcessorPybind.hh"
//#include "Utility/KTConfigurablePybind.hh"

PYBIND11_MODULE( py_nymph, mod )
{
    //Nymph::ExportKTConfigurable( mod );
    Nymph::ExportKTProcessor( mod );
    Nymph::ExportKTProcessorToolbox( mod );
}



