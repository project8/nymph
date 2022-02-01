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
            fAfterCutSignal("all", this),
            fAfterCutPassSignal("pass", this),
            fAfterCutFailSignal("fail", this)
    {
        fFilterDataSW = RegisterSlot("filter", this, &KTCutFilter::FilterData, {"all", "pass", "fail"});
    }

    KTCutFilter::~KTCutFilter()
    {
    }

    bool KTCutFilter::Configure(const scarab::param_node& node)
    {
        // Config-file settings
        if (node.has("cut-mask-int"))
        {
            SetCutMask(node["cut-mask-int"]().as_uint());
        }
        if (node.has("cut-mask"))
        {
            SetCutMask(KTCutStatus::bitset_type(node["cut-mask"]().as_string()));
        }
        if (node.get_value("cut-mask-all", false))
        {
            SetCutMaskAll();
        }

        return true;
    }

    bool KTCutFilter::Filter(const KTCoreData& data)
    {
        if (fAllBits)
        {
            return data.CutStatus().IsCut();
        }

        const KTCutStatus& cutStatus = data.CutStatus();
        if (fConvertToBitset)
        {
            fCutMask = cutStatus.ToBitset(fCutMaskInt);
        }

        if (fCutMask.size() != cutStatus.size()) fCutMask.resize(cutStatus.size());

        return cutStatus.IsCut(fCutMask);
    }

    void KTCutFilter::FilterData(KTDataHandle dataHandle)
    {
        std::shared_ptr< KTThreadReference > ref = fFilterDataSW->GetThreadRef();

        // all KTDataHandle's have KTCoreData, so we won't bother checking

        bool failCut = Filter(dataHandle->Of< KTCoreDataExt >());

        ref->Break( dataHandle, fFilterDataSW->GetDoBreakpoint() );

        if (failCut)
        {
            fAfterCutFailSignal(dataHandle);
        }
        else
        {
            fAfterCutPassSignal(dataHandle);
        }
        fAfterCutSignal(dataHandle);

        return;
    }



} /* namespace Nymph */
