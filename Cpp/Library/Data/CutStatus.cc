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

    // Ben: above is constructors for  different argument cases (empty, const cutstatus, reference). Shouldn't need to change.

    // Ben: This one needs to change. shouldn't need mask with maps.
/*    void CutStatus::AssignCutResult(unsigned maskPos, const std::string& name, bool state, bool doUpdateStatus)
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
*/
    // Ben: map version. Not sure if need the doUpdateStatus part tho
    void CutStatus::AssignCutResult(const std::string& name, bool state, bool doUpdateStatus)
    {
        LDEBUG(cutlog, "Assigning cut result <" << name << "> with state <" << state << ">");
        // Ben: don't think I need the resize stuff, happens automatically with maps
        if( fCutResults.find(name) != fCutResults.end())
        {
            throw Exception() << "Cut with name: " << name << " has already been assigned";
        }
        fCutResults.insert(make_pair(name,state));
        LDEBUG(cutlog, "Cut result size is now: " << fCutResults.size() );
        if( doUpdateStatus ) UpdateStatus();
        return;
    } // Ben: Should be fully adjusted, might need fixing for the update status stuff

    // Ben: displays map keys and vals with ostream. 
    void CutStatus::UpdateStatus()
    {
        LDEBUG(cutlog, "Updating cut summary");
        unsigned nCuts = fCutResults.size();
        //fSummary.resize(nCuts, false);  // now just a string

        // loop through to set cuts
	LDEBUG(cutlog, "nCuts: " << nCuts);
        for (unsigned iCut = 0; iCut < nCuts; ++iCut) // I think this still works for map. I believe map[iterator] bring up ith key/value
        {
            //fSummary[iCut] = fCutResults[iCut].fName && fCutResults[iCut].fState;
            if(fCutResults[iCut]->second == 1) // may need to change iCut to iterator type
            {
                fSummary.append(fCutResults[iCut]->first;
            }
        }
        LDEBUG(cutlog, "Cut summary: " << fSummary);
        return;
    }

    std::string CutStatus::CutResultsPresent() const
    {
        std::string cutsPresent;
        for (auto cutIt = fCutResults.cbegin(); cutIt != fCutResults.cend(); ++cutIt)
        {
            if ((! cutIt->first.empty()) && (cutIt->second == 1))
            {
                cutsPresent = cutIt->first + " " + cutsPresent; // TODO still need to change this to using vector of strings?
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
