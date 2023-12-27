/*
 * NymphPybind.cc
 *
 *  Created on: Feb 14, 2022
 *      Author: F. Thomas
 */

#include <pybind11/pybind11.h>

#include "Data/DataPybind.hh"
#include "Data/DataFramePybind.hh"
#include "Processor/ProcessorPybind.hh"
#include "Processor/ProcessorToolboxPybind.hh"
#include "Processor/PyProcCreatorPybind.hh"
#include "Processor/SignalPybind.hh"
#include "Processor/SlotPybind.hh"

#include "DataFrame.hh"

namespace py = pybind11;


PYBIND11_MODULE(_nymph, nymphPackage) 
{

    nymphPackage.doc() = "Nymph package";

    auto nymphControl = nymphPackage.def_submodule("control", "Control module");

    auto nymphData = nymphPackage.def_submodule("data", "Data module");
    NymphPybind::ExportData(nymphData);
    NymphPybind::ExportDataFrame(nymphData);
    
    auto nymphImplementation = nymphPackage.def_submodule("implementation", "Implementation module");

    auto nymphProcessor = nymphPackage.def_submodule("processor", "Processor module");
    NymphPybind::ExportProcessor(nymphProcessor);
    NymphPybind::ExportProcessorToolbox(nymphProcessor);
    NymphPybind::ExportPyProcCreator(nymphProcessor);
    NymphPybind::ExportSlot(nymphProcessor);
    NymphPybind::ExportSignalBase(nymphProcessor);
    NymphPybind::ExportSignal<Nymph::DataHandle>(nymphProcessor, "Data");
    
    auto nymphUtility = nymphPackage.def_submodule("utility", "Utility module");
}
