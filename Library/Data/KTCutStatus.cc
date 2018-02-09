/*
 * KTCut.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "KTCutStatus.hh"

#include "KTExtensibleStructFactory.hh"
#include "KTLogger.hh"

namespace Nymph
{
    KTLOGGER(cutlog, "KTCut");

    KTCutStatus::KTCutStatus() :
            fCutResults(),
            fSummary()
    {
    }

    KTCutStatus::KTCutStatus(const KTCutStatus& orig) :
            fCutResults(orig.fCutResults),
            fSummary()
    {
        UpdateStatus();
    }

    KTCutStatus::~KTCutStatus()
    {}

    KTCutStatus& KTCutStatus::operator=(const KTCutStatus& rhs)
    {
        fCutResults = rhs.fCutResults;
        UpdateStatus();
        return *this;
    }

    void KTCutStatus::AssignCutResult(unsigned maskPos, const std::string& name, bool state, bool doUpdateStatus)
    {
        if( fCutResults[maskPos].fAssigned )
        {
            throw KTException() << "Mask position <" << maskPos << "> has already been assigned";
        }
        fCutResults[maskPos].fName = name;
        fCutResults[maskPos].fState = state;
        fCutResults[maskPos].fAssigned = true;
        if( doUpdateStatus ) UpdateStatus();
        return;
    }

    void KTCutStatus::UpdateStatus()
    {
        KTDEBUG(cutlog, "Updating cut summary");
        unsigned nCuts = fCutResults.size();
        fSummary.resize(nCuts, false);

        // loop through to set cuts
        for (unsigned iCut = 0; iCut < nCuts; ++iCut)
        {
            fSummary[iCut] = fCutResults[iCut].fAssigned && fCutResults[iCut].fState;
        }
        KTDEBUG(cutlog, "Cut summary bitset: " << fSummary);
        return;
    }

    std::string KTCutStatus::CutResultsPresent() const
    {
        std::string cutsPresent;
        for (auto cutIt = fCutResults.cbegin(); cutIt != fCutResults.cend(); ++cutIt)
        {
            if (! cutIt->fName.empty())
            {
                cutsPresent = cutIt->fName + " " + cutsPresent;
            }
        }
        return cutsPresent;
    }

    std::ostream& operator<<(std::ostream& out, const KTCutStatus& status)
    {
        out << "Cut summary: " << status.fSummary << '\n';
        return out;
    }


} /* namespace Nymph */
