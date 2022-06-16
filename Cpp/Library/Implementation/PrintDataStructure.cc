/*
 * PrintDataStructure.cc
 *
 *  Created on: Sept 12, 2014
 *      Author: N.S. Oblath
 *  Modified to match nymph 2 convention by Yuhao Sun 2022 06 16
 */

#include "PrintDataStructure.hh"

#include "Logger.hh"

#include <sstream>

using boost::shared_ptr;

namespace Nymph
{
    LOGGER(datalog, "PrintDataStructure");

    // Register the processor
    REGISTER_PROCESSOR(PrintDataStructure, "print-data-structure");

    PrintDataStructure::PrintDataStructure(const std::string& name) :
            Processor(name),
            fDataSignal("data", this),
            fDataStructSlot("print-data", this, &PrintDataStructure::PrintDataStructure, {"data"}),
            fCutStructSlot("print-cuts", this, &PrintDataStructure::PrintCutStructure, {"data"}),
            fDataAndCutStructSlot("print-data-and-cuts", this, &PrintDataStructure::PrintDataAndCutStructure, {"data"})
    {
    }

    PrintDataStructure::~PrintDataStructure()
    {
    }

    void PrintDataStructure::Configure(const scarab::param_node&)
    {
        return;
    }

    void PrintDataStructure::PrintDataStructure(DataHandle dataHandle)
    {
        DoPrintDataStructure(dataHandle);

        SlotWrapper* slotWrap = fDataStructSlot.GetSlotWrapper();
        slotWrap->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }

    void PrintDataStructure::PrintCutStructure(DataHandle dataHandle)
    {
        DoPrintCutStructure(dataHandle);

        SlotWrapper* slotWrap = fCutStructSlot.GetSlotWrapper();
        fCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }


    void PrintDataStructure::PrintDataAndCutStructure(DataHandle dataHandle)
    {
        DoPrintDataStructure(dataHandle);
        DoPrintCutStructure(dataHandle);

        SlotWrapper* slotWrap = fDataAndCutStructSlot.GetSlotWrapper();
        fDataAndCutStructSlot.GetSlotWrapper()->GetThreadRef()->Break( dataHandle, slotWrap->GetDoBreakpoint() );

        fDataSignal(dataHandle);

        return;
    }

    void PrintDataStructure::DoPrintDataStructure(DataHandle dataHandle)
    {
        std::stringstream printbuf;

        printbuf << "\nData Structure:\n";
        printbuf << "\t- " << dataHandle->Name() << '\n';
        DEBUG(datalog, "Found data type " << dataHandle->Name());
        ExtensibleCore< DataRider >::BasePtrType nextData = dataHandle->Next();
        while (nextData != NULL)
        {
            printbuf << "\t- " << nextData->Name() << '\n';
            DEBUG(datalog, "Found data type " << nextData->Name());
            nextData = nextData->Next();
        }

        INFO(datalog, printbuf.str());

        return;
    }

    void PrintDataStructure::DoPrintCutStructure(DataHandle dataHandle)
    {
        std::stringstream printbuf;

        CutStatus& cutStatus = dataHandle->CutStatus();
        printbuf << "\n" << cutStatus;

        const CutStatus::CutResults_t cutResults = cutStatus.CutResults();
        printbuf << "Cut Structure:\n";
        unsigned nCuts = cutResults.size();
        for (unsigned iCut = 0; iCut < nCuts; ++iCut)
        {
            if (! cutResults[iCut].fAssigned) continue;
            printbuf << "\t" << iCut << " -- " << cutResults[iCut].fName << " -- is cut: " << cutResults[iCut].fState << '\n';
            DEBUG(datalog, "Found cut type " << cutResults[iCut].fName << " at mask position " << iCut);
        }

        INFO(datalog, printbuf.str());

        return;
    }

} /* namespace Nymph */
