/*
 * CutStatus.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "CutStatus.hh"
//#include "KTExtensibleStructFactory.hh"
#include "logger.hh"

namespace Nymph
{
    LOGGER(cutlog, "Cut");

//    CutStatus::CutStatus() :
//            CutResults(),
//    {
//    }

//    CutStatus::CutStatus(const CutStatus& orig) :
//            CutResults(orig.CutResults),
//    {
//    }

    CutStatus::~CutStatus()
    {}

    CutStatus& CutStatus::operator=(const CutStatus& rhs)
    {
        fCutResults = rhs.fCutResults;
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
    void CutStatus::AssignCutResult(const std::string& cutName, bool state)
    {
        LDEBUG(cutlog, "Assigning cut result <" << cutName << "> with state <" << state << ">");
        // Ben: don't think I need the resize stuff, happens automatically with maps
        if( fCutResults.find(cutName) != fCutResults.end())
        {
            throw Exception() << "Cut with name: " << cutName << " has already been assigned";
        }
        fCutResults.insert(make_pair(cutName,state));
        LDEBUG(cutlog, "Cut result size is now: " << fCutResults.size() );
        return;
    } 

    // Ben: displays map keys and vals with ostream. 
/*    void CutStatus::UpdateStatus()
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
    }*/

    std::vector< std::string > CutStatus::CutResultsPresent() const
    {
        std::vector< std::string > cutsPresent;
        for (auto cutIt = fCutResults.cbegin(); cutIt != fCutResults.cend(); ++cutIt)
        {
            if ((! cutIt->first.empty()) && (cutIt->second == 1))
            {
                //cutsPresent = cutIt->first + " " + cutsPresent; // TODO still need to change this to using vector of strings?
                cutsPresent.push_back(cutIt->first);
            }
        }
        return cutsPresent;
    }

    std::ostream& operator<<(std::ostream& out, const CutStatus& status)
    {
        out << "Cut summary: " << '\n';
        std::vector< std::string > cuts = status.CutResultsPresent();
        for (auto cutIt = cuts.cbegin(); cutIt != cuts.cend(); ++cutIt)
        {
            //out << cuts[cutIt] << "; ";
            out << *cutIt << "; ";
        }
        out << '\n';
        return out;
    }


} /* namespace Nymph */
