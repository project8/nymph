/*
 * CutFilter.cc
 *
 *  Created on: Oct 06, 2014
 *      Author: nsoblath
 */

#include "CutFilter.hh"

#include "Cut.hh"

using std::string;


namespace Nymph
{
    LOGGER(cutlog, "CutFilter");

    REGISTER_PROCESSOR(CutFilter, "cut-filter");

    CutFilter::CutFilter(const std::string& name) :
            Processor(name),
            fCutMask(),
            fCutMaskInt(0),
            fConvertToBitset(false),
            fAllBits(true),
            fAfterCutSignal("all", this),
            fAfterCutPassSignal("pass", this),
            fAfterCutFailSignal("fail", this)
    {
        //fFilterDataSW = RegisterSlot("filter", this, &CutFilter::FilterData, {"all", "pass", "fail"});
	Slot FilterSlot("filter", this, &CutFilter::FilterData);
	RegisterSlot("filter", &FilterSlot); 
    }

    CutFilter::~CutFilter()
    {
    }

    void CutFilter::Configure(const scarab::param_node& node)
    {
        // Config-file settings
        if (node.has("cut-mask-int"))
        {
            SetCutMask(node["cut-mask-int"]().as_uint());
        }
        if (node.has("cut-mask"))
        {
            SetCutMask(CutStatus::bitset_type(node["cut-mask"]().as_string()));
        }
        if (node.get_value("cut-mask-all", false))
        {
            SetCutMaskAll();
        }
    }
/*
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
*/
    void CutFilter::FilterData(DataHandle dataHandle)
    {
/*
        std::shared_ptr< KTThreadReference > ref = fFilterDataSW->GetThreadRef();

        // all KTDataHandle's have KTCoreData, so we won't bother checking

        bool failCut = Filter(dataHandle->Of< KTCoreDataExt >());

        ref->Break( dataHandle, fFilterDataSW->GetDoBreakpoint() );
*/
/*
        if (failCut)
        {
            fAfterCutFailSignal(dataHandle);
        }
        else
        {
            fAfterCutPassSignal(dataHandle);
        }
*/
        fAfterCutSignal(dataHandle);

        return;
    }



} /* namespace Nymph */
