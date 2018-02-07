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
            fCutResults(dynamic_cast< KTCutResultHandle* >(orig.fCutResults->Clone())),
            fSummary()
    {
        UpdateStatus();
    }

    KTCutStatus::~KTCutStatus()
    {}

    KTCutStatus& KTCutStatus::operator=(const KTCutStatus& rhs)
    {
        fCutResults.reset(dynamic_cast< KTCutResultHandle* >(rhs.fCutResults->Clone()));
        UpdateStatus();
        return *this;
    }

    void KTCutStatus::UpdateStatus()
    {
        KTDEBUG(cutlog, "Updating cut summary");
        KTCutResult* cut = fCutResults.get()->Next(); // skip over KTCutResultHandle
        if (cut == NULL)
        {
            KTDEBUG(cutlog, "No cuts");
            fSummary.resize(1, false);
            return;
        }

        // loop through once to count cuts
        unsigned nCuts = 0;
        while (cut != NULL)
        {
            ++nCuts;
            cut = cut->Next();
        }
        KTDEBUG(cutlog, nCuts << " cuts");
        fSummary.resize(nCuts, false);
        // loop through again to set cuts
        cut = fCutResults.get()->Next(); // skip over KTCutResultHandle
        for (unsigned iCut = 0; iCut < nCuts; ++iCut)
        {
            fSummary[iCut] = cut->GetState();
            cut = cut->Next();
        }
        KTDEBUG(cutlog, "Cut summary bitset: " << fSummary);
        return;
    }

    std::string KTCutStatus::CutResultsPresent() const
    {
        KTCutResult* cut = fCutResults.get()->Next(); // skip over KTCutResultHandle
        if (cut == NULL ) return "";

        std::string cutsPresent;
        for (auto cutIt = fCutResults.cbegin(); cutIt != fCutResults.cend(); ++cutIt)
        {
            cutsPresent = cutIt->Name() + cutsPresent;
        }
        while (true)
        {
            cutsPresent = cut->Name() + cutsPresent;
            cut = cut->Next();
            if (cut != NULL) cutsPresent = " " + cutsPresent;
            else break;
        }
        return cutsPresent;
    }

    std::ostream& operator<<(std::ostream& out, const KTCutStatus& status)
    {
        out << "Cut summary: " << status.fSummary << '\n';
        return out;
    }


} /* namespace Nymph */
