/*
 * NymphPybind.cc
 *
 *  Created on: Jan 27, 2018
 *      Author: obla999
 */

#include "Application/KTProcessorToolboxPybind.hh"
#include "Processor/KTProcessorPybind.hh"
#include <memory>
//#include "Utility/KTConfigurablePybind.hh"

PYBIND11_MODULE( py_nymph, mod )
{
    //Nymph::ExportKTConfigurable( mod );
	//Nymph::ExportKTProcessor( mod );
    Nymph::ExportKTProcessorToolbox( mod );

    pybind11::class_< Nymph::KTProcessor, std::shared_ptr<Nymph::KTProcessor>>( mod, "KTProcessor" );

    pybind11::class_<Nymph::KTWrapProcessor, Nymph::KTPyWrapProcessor, Nymph::KTProcessor, std::shared_ptr<Nymph::KTWrapProcessor>> wrap_processor(mod, "WrapProcessor", pybind11::dynamic_attr());
            wrap_processor
				.def(pybind11::init<>())
				.def("WrapFunction", &Nymph::KTWrapProcessor::WrapFunction)
				.def("Configure", &Nymph::KTWrapProcessor::Configure);

}



