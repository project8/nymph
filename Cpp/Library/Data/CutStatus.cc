/*
 * CutStatus.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "CutStatus.hh"
#include "logger.hh"

namespace Nymph
{
    LOGGER(cutlog, "Cut");

    CutStatus::CutStatus()
    {
    }

    CutStatus::CutStatus(const CutStatus& orig)
    {
            fCutResults = orig.fCutResults;
    }

    CutStatus::~CutStatus()
    {}

    CutStatus& CutStatus::operator=(const CutStatus& rhs)
    {
        //delete[] fCutResults;
        fCutResults = rhs.fCutResults;
        return *this;
    }
/*
    CutStatus CutStatus::operator=(const CutStatus rhs)
    {
        fCutResults = rhs.fCutResults;
        return *this;
    }
*/
    void CutStatus::AssignCutResult(const std::string& cutName, bool state)
    {
        LDEBUG(cutlog, "Assigning cut result <" << cutName << "> with state <" << state << ">");
        if( fCutResults.find(cutName) != fCutResults.end())
        {
            throw Exception() << "Cut with name: " << cutName << " has already been assigned";
        }
        fCutResults.insert(make_pair(cutName,state));
        LDEBUG(cutlog, "Cut result size is now: " << fCutResults.size() );
        return;
    } 

    std::vector< std::string > CutStatus::CutResultsPresent() const
    {
        std::vector< std::string > cutsPresent;
        for (auto cutIt = fCutResults.cbegin(); cutIt != fCutResults.cend(); ++cutIt)
        {
            if ((! cutIt->first.empty()) && (cutIt->second == 1))
            {
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
            out << *cutIt << "; ";
        }
        out << '\n';
        return out;
    }


} /* namespace Nymph */
