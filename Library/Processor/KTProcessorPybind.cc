/*
 * KTProcessorPybind.cc
 *
 *  Created on: Feb 06, 2018
 *      Author: cclaessens
 */

#include "KTProcessor.hh"
#include "KTProcessorPybind.hh"


#include "KTLogger.hh"

namespace Nymph
{

	KTLOGGER(procpylog, "KTProcessorPybind");
	KT_REGISTER_PROCESSOR(KTWrapProcessor, "base-wrap-proc");

    KTWrapProcessor::KTWrapProcessor( const std::string& name ) :
            KTProcessor( name ),
            fSlot("wrap-slot", this, &KTWrapProcessor::SlotFunc)

    {
    }

    KTWrapProcessor::~KTWrapProcessor()
    {
    }

    bool KTWrapProcessor::Configure(const scarab::param_node&)
    {
        return true;
    }

    void KTWrapProcessor::SlotFunc(int input)
    {
        KTINFO(procpylog, "Calling wrap function ");
        this->WrapFunction(input);
        return;
    }

    void KTWrapProcessor::WrapFunction(int input)
    {
        KTINFO(procpylog, "Hey, I'm base class");
        return;
    }
} /* namespace Nymph */
