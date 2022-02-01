/*
 * KTProcessorTemplate.cc
 *
 *  Created on: [date]
 *      Author: [name]
 */

#include "KTProcessorTemplate.hh"

#include "KTLogger.hh"
#include "KTNOFactory.hh"
#include "scarab::param.hh"


namespace Nymph
{
    KTLOGGER([logger], "KTProcessorTemplate");

    // Register the processor
    KT_REGISTER_PROCESSOR(KTProcessorTemplate, "[config-name]");

    KTProcessorTemplate::KTProcessorTemplate(const std::string& name) :
            KTProcessor(name)
            // initialize all member variables
            // initialize signals:
            // f[SomeName]Signal("[signal-name]", this)
            // initialize slots:
            // fHeaderSlot("header", this, &KTProcessorTemplate::[function to call with header]),
            // f[SomeName]Slot("[slot-name]", this, &KTProcessorTemplate::[function to call], &f[SomeName]Signal)
    {
    }

    KTProcessorTemplate::~KTProcessorTemplate()
    {
    }

    bool KTProcessorTemplate::Configure(const scarab::param_node& node)
    {
        // Configure parameters

        return true;
    }

    // All the normal stuff goes here

} /* namespace Nymph */
