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

    bool KTPrintDataStructure::Configure(const scarab::param_node* node)
    {
        return true;
    }

    void KTPrintDataStructure::PrintDataStructure(KTDataPtr dataPtr)
    {
        DoPrintDataStructure(dataPtr);

        KTSlotWrapper* slotWrap = fDataStructSlot.GetSlotWrapper();
        slotWrap->GetThreadRef()->Break( dataPtr, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataPtr);

        return;
    }

    void KTPrintDataStructure::PrintCutStructure(KTDataPtr dataPtr)
    {
        DoPrintCutStructure(dataPtr);

        KTSlotWrapper* slotWrap = fCutStructSlot.GetSlotWrapper();
        fCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataPtr, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataPtr);

        return;
    }


    void KTPrintDataStructure::PrintDataAndCutStructure(KTDataPtr dataPtr)
    {
        DoPrintDataStructure(dataPtr);
        DoPrintCutStructure(dataPtr);

        KTSlotWrapper* slotWrap = fDataAndCutStructSlot.GetSlotWrapper();
        fDataAndCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataPtr, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataPtr);

        return;
    }

    void KTPrintDataStructure::DoPrintDataStructure(KTDataPtr dataPtr)
    {
        std::stringstream printbuf;

        printbuf << "\nData Structure:\n";
        printbuf << "\t- " << dataPtr->Name() << '\n';
        KTDEBUG(datalog, "Found data type " << dataPtr->Name());
        KTExtensibleStructCore< KTDataCore >* nextData = dataPtr->Next();
        while (nextData != NULL)
        {
            printbuf << "\t- " << nextData->Name() << '\n';
            KTDEBUG(datalog, "Found data type " << nextData->Name());
            nextData = nextData->Next();
        }

        KTINFO(datalog, printbuf.str());

        return;
    }

    void KTPrintDataStructure::DoPrintCutStructure(KTDataPtr dataPtr)
    {
        std::stringstream printbuf;

        KTCutStatus& cutStatus = dataPtr->CutStatus();
        printbuf << "\n" << cutStatus;

        const KTCutResult* cutResult = cutStatus.CutResults();
        printbuf << "Cut Structure:\n";
        while (cutResult != NULL)
        {
            printbuf << "\t- " << cutResult->Name() << " -- is cut: " << cutResult->GetState() << '\n';
            KTDEBUG(datalog, "Found cut type " << cutResult->Name());
            cutResult = cutResult->Next();
        }

        KTINFO(datalog, printbuf.str());

        return;
    }

} /* namespace Nymph */
