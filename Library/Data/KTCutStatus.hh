/*
 * KTCutStatus.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef KTCUTSTATUS_HH_
#define KTCUTSTATUS_HH_


#include "KTCutResult.hh"

#include "KTException.hh"

#include <boost/dynamic_bitset.hpp>

#include <algorithm>
#include <memory>
#include <vector>

namespace Nymph
{
    /*!
     @class KTCutStatus
     @author N. S. Oblath

     @brief Provides easy access to cut information.

     @details
     KTCutStatus is typically used as a member variable of KTCoreData, the top-level data object.

     KTCutStatus owns the set of cut results that have been added to a data object.
     It also owns a summary of those cuts (implemented with boost::dynamic_bitset).

     You can check if the data has been cut with the IsCut functions.
     - IsCut() returns true if any cut results are true;
     - IsCut(const bitset_type& mask), IsCut(unsigned int mask), and IsCut(const std::string& mask) allow you to specify
       a cut mask, and return true if any of the cut results specified by the mask are true.

     When specifying a cut mask, bits set to true specify cuts that should be used:
     - bitset_type is boost::dynamic_bitset;
     - unsigned integer masks use the bits of the integer;
     - std::string masks are strings with each character either a 0 or 1.

     With KTCutStatus you can interact with individual cut results in the following ways:
     - Add cut results to a data object with AddCutResult,
     - Check to see if a particular cut result is present using HasCutResult,
     - Get the value of a cut result with GetCutState,
     - Set the value of a cut result with SetCutState,
     - Directly access the cut result with GetCutResult, and
     - Remove a cut result with RemoveCutResult.

     For all except KTCutStatus::RemoveCutResult, the cut result can be identified by type or string name.
     */

    class KTCutStatus
    {
        public:
            typedef boost::dynamic_bitset< > bitset_type;

            typedef std::vector< KTCutResult > CutResults_t;

            typedef CutResults_t::iterator CutResultsIt;
            typedef CutResults_t::const_iterator CutResultsCIt;

        public:
            KTCutStatus();
            KTCutStatus(const KTCutStatus& orig);
            ~KTCutStatus();

            KTCutStatus& operator=(const KTCutStatus& rhs);

            /// Returns the size of the cut results vector
            size_t size() const;

            /// Returns a reference to the cut results vector
            const CutResults_t& CutResults() const;

            /// Updates the summary bitset from the cut results vector
            void UpdateStatus();

            /// Adds a new cut result if it doesn't exist, and assigns the specified name and state at maskPos; throws KTException if maskPos is already assigned
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

            /// Returns the state of the named cut; throws KTException if it doesn't exist
            bool GetCutState(const std::string& cutName) const;
            /// Returns the state of the specified cut; throws KTException if it doesn't exist
            bool GetCutState(unsigned maskPos) const;

            /// Sets the state of the specified cut; throws KTException if it doesn't exist
            void SetCutState(const std::string& cutName, bool state, bool doUpdateStatus=true);
            /// Sets the state of the specified cut; throws KTException if it doesn't exist
            void SetCutState(unsigned maskPos, bool state, bool doUpdateStatus=true);

            /// Returns a string with the names of the cuts that are present in bitset order
            std::string CutResultsPresent() const;


        private:
            friend std::ostream& operator<<(std::ostream& out, const KTCutStatus& status);

            CutResults_t fCutResults;

            bitset_type fSummary;

        public:
            bool IsCut() const;
            bool IsCut(const bitset_type& mask) const;
            bool IsCut(unsigned long long mask) const;
            bool IsCut(const std::string& mask) const;

            bitset_type ToBitset(unsigned long long mask) const;
            bitset_type ToBitset(const std::string& mask) const;

    };

    std::ostream& operator<<(std::ostream& out, const KTCutStatus& status);


    inline const KTCutStatus::CutResults_t& KTCutStatus::CutResults() const
    {
        return fCutResults;
    }

    inline KTCutStatus::CutResultsIt KTCutStatus::FindCutResult( const std::string& name )
    {
        if( name.empty() ) return fCutResults.end();
        return std::find_if( fCutResults.begin(), fCutResults.end(), CheckCutResultName(name) );
    }

    inline KTCutStatus::CutResultsCIt KTCutStatus::FindCutResultC( const std::string& name ) const
    {
        if( name.empty() ) return fCutResults.cend();
        return std::find_if( fCutResults.cbegin(), fCutResults.cend(), CheckCutResultName(name) );
    }

    inline KTCutStatus::CutResultsIt KTCutStatus::CutResultsEnd()
    {
        return fCutResults.end();
    }

    inline KTCutStatus::CutResultsCIt KTCutStatus::CutResultsEndC() const
    {
        return fCutResults.cend();
    }

    inline bool KTCutStatus::HasCutResult( const std::string& name ) const
    {
        return FindCutResultC(name) != std::end(fCutResults);
    }

    inline bool KTCutStatus::HasCutResult( unsigned maskPos ) const
    {
        return maskPos < fCutResults.size() && fCutResults[maskPos].fAssigned;
    }

    inline bool KTCutStatus::GetCutState( const std::string& name ) const
    {
        CutResultsCIt cutIt = FindCutResultC(name);
        if (cutIt != fCutResults.cend())
        {
            return cutIt->fState;
        }
        throw KTException() << "Unable to find cut <" << name << ">";
    }

    inline bool KTCutStatus::GetCutState( unsigned maskPos ) const
    {
        if (maskPos < fCutResults.size())
        {
            return fCutResults[maskPos].fState;
        }
        throw KTException() << "Mask position <" << maskPos << "> is out of range (only " << size() << " cuts are present)";
    }

    inline void KTCutStatus::SetCutState(const std::string& name, bool state, bool doUpdateStatus)
    {
        CutResultsIt cutIt = FindCutResult(name);
        if (cutIt != fCutResults.end())
        {
            cutIt->fState = state;
            if (doUpdateStatus) UpdateStatus();
            return;
        }
        throw KTException() << "Unable to find cut <" << name << ">";
    }

    inline void KTCutStatus::SetCutState(unsigned maskPos, bool state, bool doUpdateStatus)
    {
        if (maskPos < fCutResults.size())
        {
            fCutResults[maskPos].fState = state;
            if (doUpdateStatus) UpdateStatus();
            return;
        }
        throw KTException() << "Mask position <" << maskPos << "> is out of range (only "<< size() << " cuts are present)";
    }

    inline void KTCutStatus::RemoveCutResult(const std::string& name, bool doUpdateStatus)
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

    inline void KTCutStatus::RemoveCutResult(unsigned maskPos, bool doUpdateStatus)
    {
        if (maskPos < fCutResults.size())
        {
            fCutResults[maskPos].fName = "";
            fCutResults[maskPos].fState = false;
        }
        if (doUpdateStatus) UpdateStatus();
        return;
    }

    inline size_t KTCutStatus::size() const
    {
        return fSummary.size();
    }

    inline bool KTCutStatus::IsCut() const
    {
        return fSummary.any();
    }

    inline bool KTCutStatus::IsCut(const bitset_type& mask) const
    {
        return (fSummary & mask).any();
    }

    inline bool KTCutStatus::IsCut(unsigned long long mask) const
    {
        return IsCut(ToBitset(mask));
    }

    inline bool KTCutStatus::IsCut(const std::string& mask) const
    {
        return IsCut(ToBitset(mask));
    }

    inline KTCutStatus::bitset_type KTCutStatus::ToBitset(unsigned long long mask) const
    {
        return bitset_type(fSummary.size(), mask);
    }

    inline KTCutStatus::bitset_type KTCutStatus::ToBitset(const std::string& mask) const
    {
        return bitset_type(mask);
    }

} /* namespace Nymph */

#endif /* KTCUTSTATUS_HH_ */
