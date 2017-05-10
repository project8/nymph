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
        //.def("RegisterProcessor", pure_virtual(&KTProcessor::RegisterProcessor), return_value_policy<manage_new_object>(), "register a processor")
        
        // PassThreadRefUpdate... needs KTThreadReference
        //.def("PassThreadRefUpdate", &KTProcessor::PassThreadRefUpdate) 
        .def("ConnectASlot", &KTProcessor::ConnectASlot)
        .def("ConnectASignal", &KTProcessor::ConnectASignal)

        // these next four need wrappers for return types (KT[Signal,Slot]Wrapper)
        //.def("RegisterSignal", &KTProcessor::RegisterSignal)
        //.def("RegisterSlot", &KTProcessor::RegisterSlot)
        //.def("GetSignal", &KTProcessor::GetSignal)
        //.def("GetSlot", &KTProcessor::GetSlot)

        /* now protected */
        //.def("ConnectSignalToSlot", &KTProcessor::ConnectSignalToSlot)

        .def("GetDoBreakpoint", &KTProcessor::GetDoBreakpoint)
        .def("SetDoBreakpoint", &KTProcessor::SetDoBreakpoint)

        /* Inherited methods from unwrapped bases */
        .def("Configure", Configure_JsonStr, "Configure from json encoded configuration")
        ;
}

#endif /* KTPROCESSORPY_HH_ */
