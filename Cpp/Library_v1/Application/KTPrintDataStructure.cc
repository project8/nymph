/*
 * KTPrintDataStructure.cc
 *
 *  Created on: Sept 12, 2014
 *      Author: N.S. Oblath
 */

#include "KTPrintDataStructure.hh"

#include "KTLogger.hh"

#include <sstream>

using boost::shared_ptr;

namespace Nymph
{
    KTLOGGER(datalog, "KTPrintDataStructure");

    // Register the processor
    KT_REGISTER_PROCESSOR(KTPrintDataStructure, "print-data-structure");

    KTPrintDataStructure::KTPrintDataStructure(const std::string& name) :
            KTProcessor(name),
            fDataSignal("data", this),
            fDataStructSlot("print-data", this, &KTPrintDataStructure::PrintDataStructure, {"data"}),
            fCutStructSlot("print-cuts", this, &KTPrintDataStructure::PrintCutStructure, {"data"}),
            fDataAndCutStructSlot("print-data-and-cuts", this, &KTPrintDataStructure::PrintDataAndCutStructure, {"data"})
    {
    }

    KTPrintDataStructure::~KTPrintDataStructure()
    {
    }

    bool KTPrintDataStructure::Configure(const scarab::param_node&)
    {
        return true;
    }

    void KTPrintDataStructure::PrintDataStructure(KTDataHandle dataHandle)
    {
        DoPrintDataStructure(dataHandle);

        KTSlotWrapper* slotWrap = fDataStructSlot.GetSlotWrapper();
        slotWrap->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }

    void KTPrintDataStructure::PrintCutStructure(KTDataHandle dataHandle)
    {
        DoPrintCutStructure(dataHandle);

        KTSlotWrapper* slotWrap = fCutStructSlot.GetSlotWrapper();
        fCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }


    void KTPrintDataStructure::PrintDataAndCutStructure(KTDataHandle dataHandle)
    {
        DoPrintDataStructure(dataHandle);
        DoPrintCutStructure(dataHandle);

        KTSlotWrapper* slotWrap = fDataAndCutStructSlot.GetSlotWrapper();
        fDataAndCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }

    void KTPrintDataStructure::DoPrintDataStructure(KTDataHandle dataHandle)
    {
        std::stringstream printbuf;

        printbuf << "\nData Structure:\n";
        printbuf << "\t- " << dataHandle->Name() << '\n';
        KTDEBUG(datalog, "Found data type " << dataHandle->Name());
        KTExtensibleCore< KTDataRider >::BasePtrType nextData = dataHandle->Next();
        while (nextData != NULL)
        {
            printbuf << "\t- " << nextData->Name() << '\n';
            KTDEBUG(datalog, "Found data type " << nextData->Name());
            nextData = nextData->Next();
        }

        KTINFO(datalog, printbuf.str());

        return;
    }

    void KTPrintDataStructure::DoPrintCutStructure(KTDataHandle dataHandle)
    {
        std::stringstream printbuf;

        KTCutStatus& cutStatus = dataHandle->CutStatus();
        printbuf << "\n" << cutStatus;

        const KTCutStatus::CutResults_t cutResults = cutStatus.CutResults();
        printbuf << "Cut Structure:\n";
        unsigned nCuts = cutResults.size();
        for (unsigned iCut = 0; iCut < nCuts; ++iCut)
        {
            if (! cutResults[iCut].fAssigned) continue;
            printbuf << "\t" << iCut << " -- " << cutResults[iCut].fName << " -- is cut: " << cutResults[iCut].fState << '\n';
            KTDEBUG(datalog, "Found cut type " << cutResults[iCut].fName << " at mask position " << iCut);
        }

        KTINFO(datalog, printbuf.str());

        return;
    }

} /* namespace Nymph */