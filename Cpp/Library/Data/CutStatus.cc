/*
 * CutStatus.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "CutStatus.hh"

//#include "KTExtensibleStructFactory.hh"
#include "Logger.hh"

namespace Nymph
{
    LOGGER(cutlog, "Cut");

    CutStatus::CutStatus() :
            fCutResults(),
            fSummary()
    {
    }

    CutStatus::CutStatus(const CutStatus& orig) :
            fCutResults(orig.fCutResults),
            fSummary()
    {
        UpdateStatus();
    }

    CutStatus::~CutStatus()
    {}

    CutStatus& CutStatus::operator=(const CutStatus& rhs)
    {
        fCutResults = rhs.fCutResults;
        UpdateStatus();
        return *this;
    }

    void CutStatus::AssignCutResult(unsigned maskPos, const std::string& name, bool state, bool doUpdateStatus)
    {
        LDEBUG(cutlog, "Assigning cut result <" << name << "> to position <" << maskPos << "> with state <" << state << ">");
        if( maskPos >= fCutResults.size() )
        {
            fCutResults.resize( maskPos + 1 );
        }
        LDEBUG(cutlog, "Cut result size is now <" << fCutResults.size() << ">");
        if( fCutResults[maskPos].fAssigned )
        {
            throw Exception() << "Mask position <" << maskPos << "> has already been assigned";
        }
        fCutResults[maskPos].fName = name;
        fCutResults[maskPos].fState = state;
        fCutResults[maskPos].fAssigned = true;
        if( doUpdateStatus ) UpdateStatus();
        return;
    }

    void CutStatus::UpdateStatus()
    {
        LDEBUG(cutlog, "Updating cut summary");
        unsigned nCuts = fCutResults.size();
        fSummary.resize(nCuts, false);

        // loop through to set cuts
        for (unsigned iCut = 0; iCut < nCuts; ++iCut)
        {
            fSummary[iCut] = fCutResults[iCut].fAssigned && fCutResults[iCut].fState;
        }
        LDEBUG(cutlog, "Cut summary bitset: " << fSummary);
        return;
    }

    std::string CutStatus::CutResultsPresent() const
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

    std::ostream& operator<<(std::ostream& out, const CutStatus& status)
    {
        out << "Cut summary: " << status.fSummary << '\n';
        return out;
    }


} /* namespace Nymph */
