/*
 * KTCutFilter.cc
 *
 *  Created on: Oct 06, 2014
 *      Author: nsoblath
 */

#include "KTCutFilter.hh"

#include "KTCut.hh"

using std::string;


namespace Nymph
{
    KTLOGGER(cutlog, "KTCutFilter");

    KT_REGISTER_PROCESSOR(KTCutFilter, "cut-filter");

    KTCutFilter::KTCutFilter(const std::string& name) :
            KTProcessor(name),
            fCutMask(),
            fCutMaskInt(0),
            fConvertToBitset(false),
            fAllBits(true),
            fAfterCutSignal("after-cut", this),
            fAfterCutPassSignal("after-cut-pass", this),
            fAfterCutFailSignal("after-cut-fail", this)
    {
        RegisterSlot("filter", this, &KTCutFilter::FilterData);
    }

    KTCutFilter::~KTCutFilter()
    {
    }

    bool KTCutFilter::Configure(const scarab::param_node* node)
    {
        // Config-file settings
        if (node == NULL) return true;

        if (node->has("cut-mask-int"))
        {
            SetCutMask(node->get_value< unsigned long long >("cut-mask-int"));
        }
        if (node->has("cut-mask"))
        {
            SetCutMask(KTCutStatus::bitset_type(node->get_value("cut-mask")));
        }
        if (node->get_value("cut-mask-all", false))
        {
            SetCutMaskAll();
        }

        return true;
    }

    bool KTCutFilter::Filter(KTData& data)
    {
        if (fAllBits)
        {
            return data.GetCutStatus().IsCut();
        }

        KTCutStatus& cutStatus = data.GetCutStatus();
        if (fConvertToBitset)
        {
            fCutMask = cutStatus.ToBitset(fCutMaskInt);
        }

        if (fCutMask.size() != cutStatus.size()) fCutMask.resize(cutStatus.size());

        return cutStatus.IsCut(fCutMask);
    }

    void KTCutFilter::FilterData(KTDataPtr dataPtr)
    {
        // all KTDataPtr's have KTData, so we won't bother checking
        if (Filter(dataPtr->Of< KTData >()))
        {
            fAfterCutFailSignal(dataPtr);
        }
        else
        {
            fAfterCutPassSignal(dataPtr);
        }
        fAfterCutSignal(dataPtr);

        return;
    }



} /* namespace Nymph */