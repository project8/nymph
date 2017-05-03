/**
 @file KTProcessorPy.hh
 @brief Contains KTProcessor python wrapping
 @details KTProcessor is the processor base class
 @author: B. H. LaRoque
 @date: April 20, 2017
 */

#ifndef KTPROCESSORPY_HH_
#define KTPROCESSORPY_HH_

#include "KTProcessor.hh"

bool (Nymph::KTConfigurable::*Configure_JsonStr)(const std::string& config) = &Nymph::KTConfigurable::Configure;

void export_Processor()
{
    using namespace Nymph;
    using namespace boost::python;

    // KTProcessor base class
    class_<KTProcessor, boost::noncopyable>("KTProcessor", no_init)
        .def("ConnectASlot", &KTProcessor::ConnectASlot)
        .def("ConnectASignal", &KTProcessor::ConnectASignal)
        .def("ConnectSignalToSlot", &KTProcessor::ConnectSignalToSlot)
        .def("Configure", Configure_JsonStr, "Configure from json encoded configuration")
        //.def("RegisterProcessor", &KTProcessor::RegisterProcessor, return_value_policy<manage_new_object>())
        ;
}

#endif /* KTPROCESSORPY_HH_ */
