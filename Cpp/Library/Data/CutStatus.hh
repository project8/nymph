/*
 * CutStatus.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef CUTSTATUS_HH_
#define CUTSTATUS_HH_


#include "CutResult.hh"

#include "Exception.hh"

//#include "cereal/access.hpp"
//#include "cereal/types/string.hpp"
//#include "cereal/types/vector.hpp"

#include <boost/dynamic_bitset.hpp>

#include <algorithm>
#include <memory>
#include <vector>

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
            typedef boost::dynamic_bitset< > bitset_type;

            typedef std::vector< CutResult > CutResults_t;

            typedef CutResults_t::iterator CutResultsIt;
            typedef CutResults_t::const_iterator CutResultsCIt;

        public:
            CutStatus();
            CutStatus(const CutStatus& orig);
            ~CutStatus();

            CutStatus& operator=(const CutStatus& rhs);

            /// Returns the size of the cut results vector
            size_t size() const;

            /// Returns a reference to the cut results vector
            const CutResults_t& CutResults() const;

            /// Updates the summary bitset from the cut results vector
            void UpdateStatus();

            /// Adds a new cut result if it doesn't exist, and assigns the specified name and state at maskPos; throws Exception if maskPos is already assigned
            void AssignCutResult(unsigned maskPos, const std::string& name, bool state=false, bool doUpdateStatus=true);

            /// Removes the cut by erasing the name and setting the state to false
            void RemoveCutResult(const std::string& cutName, bool doUpdateStatus=true);
            /// Removes the cut by erasing the name and setting the state to false
            void RemoveCutResult(unsigned maskPos, bool doUpdateStatus=true);

            CutResultsIt FindCutResult(const std::string& cutName);
            CutResultsCIt FindCutResultC(const std::string& cutName) const;

            CutResultsIt  CutResultsEnd();
            CutResultsCIt CutResultsEndC() const;

            /// Returns whether or not the specified cut is present
            bool HasCutResult(const std::string& cutName) const;
            /// Returns whether or not the specified cut is present (whether or not it has been explicitly assigned)
            bool HasCutResult(unsigned maskPos) const;

            /// Returns the state of the named cut; throws Exception if it doesn't exist
            bool GetCutState(const std::string& cutName) const;
            /// Returns the state of the specified cut; throws Exception if it doesn't exist
            bool GetCutState(unsigned maskPos) const;

            /// Sets the state of the specified cut; throws Exception if it doesn't exist
            void SetCutState(const std::string& cutName, bool state, bool doUpdateStatus=true);
            /// Sets the state of the specified cut; throws Exception if it doesn't exist
            void SetCutState(unsigned maskPos, bool state, bool doUpdateStatus=true);

            /// Returns a string with the names of the cuts that are present in bitset order
            std::string CutResultsPresent() const;

        private:
            friend std::ostream& operator<<(std::ostream& out, const CutStatus& status);

            CutResults_t fCutResults;

            bitset_type fSummary;

        public:
            bool IsCut() const;
            bool IsCut(const bitset_type& mask) const;
            bool IsCut(unsigned long long mask) const;
            bool IsCut(const std::string& mask) const;

            bitset_type ToBitset(unsigned long long mask) const;
            bitset_type ToBitset(const std::string& mask) const;

        private:
//------SerialRemoved---------
/*
            friend class cereal::access;

            template< class Archive >
            void save( Archive& ar ) const;

            template< class Archive >
            void load( Archive& ar );
*/
    };

    std::ostream& operator<<(std::ostream& out, const CutStatus& status);


    inline const CutStatus::CutResults_t& CutStatus::CutResults() const
    {
        return fCutResults;
    }

    inline CutStatus::CutResultsIt CutStatus::FindCutResult( const std::string& name )
    {
        if( name.empty() ) return fCutResults.end();
        return std::find_if( fCutResults.begin(), fCutResults.end(), CheckCutResultName(name) );
    }

    inline CutStatus::CutResultsCIt CutStatus::FindCutResultC( const std::string& name ) const
    {
        if( name.empty() ) return fCutResults.cend();
        return std::find_if( fCutResults.cbegin(), fCutResults.cend(), CheckCutResultName(name) );
    }

    inline CutStatus::CutResultsIt CutStatus::CutResultsEnd()
    {
        return fCutResults.end();
    }

    inline CutStatus::CutResultsCIt CutStatus::CutResultsEndC() const
    {
        return fCutResults.cend();
    }

    inline bool CutStatus::HasCutResult( const std::string& name ) const
    {
        return FindCutResultC(name) != std::end(fCutResults);
    }

    inline bool CutStatus::HasCutResult( unsigned maskPos ) const
    {
        return maskPos < fCutResults.size() && fCutResults[maskPos].fAssigned;
    }

    inline bool CutStatus::GetCutState( const std::string& name ) const
    {
        CutResultsCIt cutIt = FindCutResultC(name);
        if (cutIt != fCutResults.cend())
        {
            return cutIt->fState;
        }
        throw Exception() << "Unable to find cut <" << name << ">";
    }

    inline bool CutStatus::GetCutState( unsigned maskPos ) const
    {
        if (maskPos < fCutResults.size())
        {
            return fCutResults[maskPos].fState;
        }
        throw Exception() << "Mask position <" << maskPos << "> is out of range (only " << size() << " cuts are present)";
    }

    inline void CutStatus::SetCutState(const std::string& name, bool state, bool doUpdateStatus)
    {
        CutResultsIt cutIt = FindCutResult(name);
        if (cutIt != fCutResults.end())
        {
            cutIt->fState = state;
            if (doUpdateStatus) UpdateStatus();
            return;
        }
        throw Exception() << "Unable to find cut <" << name << ">";
    }

    inline void CutStatus::SetCutState(unsigned maskPos, bool state, bool doUpdateStatus)
    {
        if (maskPos < fCutResults.size())
        {
            fCutResults[maskPos].fState = state;
            if (doUpdateStatus) UpdateStatus();
            return;
        }
        throw Exception() << "Mask position <" << maskPos << "> is out of range (only "<< size() << " cuts are present)";
    }

    inline void CutStatus::RemoveCutResult(const std::string& name, bool doUpdateStatus)
    {
        CutResultsIt cutIt = FindCutResult(name);
        if (cutIt != fCutResults.end())
        {
            cutIt->fName = "";
            cutIt->fState = false;
        }
        if (doUpdateStatus) UpdateStatus();
        return;
    }

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

    inline size_t CutStatus::size() const
    {
        return fSummary.size();
    }

    inline bool CutStatus::IsCut() const
    {
        return fSummary.any();
    }

    inline bool CutStatus::IsCut(const bitset_type& mask) const
    {
        return (fSummary & mask).any();
    }

    inline bool CutStatus::IsCut(unsigned long long mask) const
    {
        return IsCut(ToBitset(mask));
    }

    inline bool CutStatus::IsCut(const std::string& mask) const
    {
        return IsCut(ToBitset(mask));
    }

    inline CutStatus::bitset_type CutStatus::ToBitset(unsigned long long mask) const
    {
        return bitset_type(fSummary.size(), mask);
    }

    inline CutStatus::bitset_type CutStatus::ToBitset(const std::string& mask) const
    {
        return bitset_type(mask);
    }

} /* namespace Nymph */

#endif /* CUTSTATUS_HH_ */
