/*
 * CutStatus.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef CUTSTATUS_HH_
#define CUTSTATUS_HH_

#include <map>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/map.hpp>

//#include "CutResult.hh"

#include "Exception.hh"

//#include "cereal/access.hpp"
//#include "cereal/types/string.hpp"
//#include "cereal/types/vector.hpp"

namespace Nymph
{
    /*!
     @class CutStatus
     @author N. S. Oblath

     @brief Provides easy access to cut result information.

     @details
     The cut results can be imagined as an array of booleans, specifying whether the cut was passed: [true, false, false, true, . . . ].

     Cuts are assigned both an array position (non-negative integer) and a name (string) before or when the results are set.

     CutStatus is typically used as a member variable of CoreData, the top-level data object.

     CutStatus owns the set of CutResults that have been added to a data object.
     It also owns a summary of those cuts (implemented with boost::dynamic_bitset).

     You can check if the data has been cut with the IsCut functions.
     - IsCut() returns true if any cut results are true;
     - IsCut(const bitset_type& mask), IsCut(unsigned int mask), and IsCut(const std::string& mask) allow you to specify
       a cut mask, and return true if any of the cut results specified by the mask are true.

     When specifying a cut mask, bits set to true specify cuts that should be used:
     - bitset_type is boost::dynamic_bitset;
     - unsigned integer masks use the bits of the integer;
     - std::string masks are strings with each character either a 0 or 1.

     With CutStatus you can interact with individual cut results in the following ways:
     - Get the number of cut results with size(),
     - Get a reference to the cut results with CutResults(),
     - If you need to manually update the cut status bitset, use UpdateStatus(),
     - Add cut results to a data object with AssignCutResult(),
     - Remove a cut result with RemoveCutResult(),
     - Check to see if a particular cut result is present using HasCutResult(),
     - Get the value of a cut result with GetCutState(), and
     - Set the value of a cut result with SetCutState(),

     Cut results can typically be accessed by either name or mask position.
     */

    class CutStatus
    {
        public:
            //CutStatus() : CutResults();
            typedef std::map< std::string, bool > CutResults_t;
            typedef CutResults_t::iterator CutResultsIt;
            typedef CutResults_t::const_iterator CutResultsCIt;
    
        //private:
            //std::map< std::string, bool > CutResults;

        public: // Ben: Below initiates operators, shouldn't need to mess with this, just in the actual operator defns how they interact with the maps.
            CutStatus();
            CutStatus(const CutStatus& orig);
            ~CutStatus();

            CutStatus& operator=(const CutStatus& rhs);

            /// Returns the size of the cut results map
            size_t size() const;

            /// Returns a reference to the cut results map
            std::map< std::string, bool > CutResults() const;

            /// Adds a new cut result with the specified name; throws Exception if a cut with that name already exists
            void AssignCutResult(const std::string& cutName, bool state=false);

            /// Removes the cut by erasing the name and setting the state to false
            void RemoveCutResult(const std::string& cutName);

            CutResultsIt FindCutResult(const std::string& cutName);
            CutResultsCIt FindCutResultC(const std::string& cutName) const;

            CutResultsIt  CutResultsEnd();
            CutResultsCIt CutResultsEndC() const;

            /// Returns whether or not the specified cut is present
            bool HasCutResult(const std::string& cutName) const;

            /// Returns the state of the named cut; throws Exception if it doesn't exist
            bool GetCutState(const std::string& cutName) const;

            /// Sets the state of the specified cut; throws Exception if it doesn't exist
            void SetCutState(const std::string& cutName, bool state); 

            /// Returns a string with the names of the cuts that are present in bitset order
            std::vector< std::string > CutResultsPresent() const;

        private:
            friend std::ostream& operator<<(std::ostream& out, const CutStatus& status);

            CutResults_t fCutResults;

            //bitset_type fSummary; // Need to change from bitset to vector of strings or similar. Can potentially skip and directly cout map vals
            //std::string fSummary; // string containing names of only cuts that have been applied, separated by _

        public:
            bool IsCut() const;
            bool IsCut(const std::string& mask) const;
            int NumCuts() const;

    };

    std::ostream& operator<<(std::ostream& out, const CutStatus& status);

    inline std::map< std::string, bool > CutStatus::CutResults() const // get a const of the map
    {
        return fCutResults;
    }

    inline CutStatus::CutResultsIt CutStatus::FindCutResult( const std::string& cutName )
    {
        return fCutResults.find(cutName);
    }

    inline CutStatus::CutResultsCIt CutStatus::FindCutResultC( const std::string& cutName ) const
    {
        return fCutResults.find(cutName);
    }

    inline CutStatus::CutResultsIt CutStatus::CutResultsEnd()
    {
        return fCutResults.end();
    }

    inline CutStatus::CutResultsCIt CutStatus::CutResultsEndC() const
    {
        return fCutResults.cend();
    }

    inline bool CutStatus::HasCutResult( const std::string& cutName ) const
    {
        return fCutResults.count(cutName);
    }

    inline bool CutStatus::GetCutState( const std::string& cutName ) const
    {
        if (fCutResults.find(cutName) != fCutResults.end())
        {
            return fCutResults.at(cutName);
        }
        throw Exception() << "Unable to find cut <" << cutName << ">";
    }
    
    inline void CutStatus::SetCutState(const std::string& cutName, bool state)
    {
        if (fCutResults.find(cutName) != fCutResults.end())
        {
            fCutResults[cutName] = state;
            return;
        }
        throw Exception() << "Unable to find cut <" << cutName << ">";
    }

    inline void CutStatus::RemoveCutResult(const std::string& cutName)
    {
        if (fCutResults.find(cutName) != fCutResults.end())
        {
            fCutResults.erase(cutName);
        }
        return;
    }
    // Ben: can remove by iterator position, but won't for consistency
/*
    inline void CutStatus::RemoveCutResult(unsigned maskPos, bool doUpdateStatus)
    {
        if (maskPos < fCutResults.size())
        {
            fCutResults[maskPos].fName = "";
            fCutResults[maskPos].fState = false;
        }
        if (doUpdateStatus) UpdateStatus();
        return;
    }
*/
    inline size_t CutStatus::size() const
    {
        return fCutResults.size();
    }

    inline bool CutStatus::IsCut() const
    {
        int stateSum = boost::accumulate(fCutResults | boost::adaptors::map_values, 0);
        return stateSum > 0;
    }
    
    inline int CutStatus::NumCuts() const
    {
        int stateSum = boost::accumulate(fCutResults | boost::adaptors::map_values, 0);
        return stateSum;
    }
} /* namespace Nymph */

#endif /* CUTSTATUS_HH_ */
