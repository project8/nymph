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

            typedef std::shared_ptr< KTCutResult > CutResultPtr;
            typedef std::vector< CutResultPtr > CutResults_t;

            typedef CutResults_t::iterator CutResultsIt;
            typedef CutResults_t::const_iterator CutResultsCIt;

        public:
            KTCutStatus();
            KTCutStatus(const KTCutStatus& orig);
            ~KTCutStatus();

            KTCutStatus& operator=(const KTCutStatus& rhs);

            const CutResults_t& CutResults() const;

            void UpdateStatus();

            template< typename XCutType >
            bool AddCutResult(bool state, bool doUpdateStatus=true);
            template< typename XCutType >
            bool AddCutResult(const XCutType& cut, bool state, bool doUpdateStatus=true);

            template< typename XCutType >
            CutResultsIt FindCutResult();
            CutResultsIt FindCutResult(const std::string& cutName);

            template< typename XCutType >
            CutResultsCIt FindCutResultC() const;
            CutResultsCIt FindCutResultC(const std::string& cutName) const;

            CutResultsIt  CutResultsEnd();
            CutResultsCIt CutResultsEndC() const;

            template< typename XCutType >
            bool HasCutResult() const;
            bool HasCutResult(const std::string& cutName) const;

            template< typename XCutType >
            bool GetCutState() const;
            bool GetCutState(const std::string& cutName) const;

            template< typename XCutType >
            bool SetCutState(bool state, bool doUpdateStatus=true);
            bool SetCutState(const std::string& cutName, bool state, bool doUpdateStatus=true);

            template< typename XCutType >
            void RemoveCutResult(bool doUpdateStatus=true);
            void RemoveCutResult(const std::string& cutName, bool doUpdateStatus=true);

            /// Returns a string with the names of the cuts that are present in bitset order
            std::string CutResultsPresent() const;

            size_t size() const;
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

    template< typename XCutType >
    bool KTCutStatus::AddCutResult(bool state, bool doUpdateStatus)
    {
        if (! HasCutResult< XCutType >())
        {
            fCutResults.emplace_back( state );
            if (doUpdateStatus) UpdateStatus();
            return true;
        }
        return false;
    }

    template< typename XCutType >
    bool KTCutStatus::AddCutResult(const XCutType&, bool state, bool doUpdateStatus)
    {
        if (! HasCutResult< XCutType >())
        {
            fCutResults.emplace_back( state );
            if (doUpdateStatus) UpdateStatus();
            return true;
        }
        return false;
    }

    template< typename XCutType >
    inline KTCutStatus::CutResultsIt KTCutStatus::FindCutResult()
    {
        return std::find_if( fCutResults.begin(), fCutResults.end(), CheckCutResultType< XCutType > );
    }

    inline KTCutStatus::CutResultsIt KTCutStatus::FindCutResult( const std::string& name )
    {
        return std::find_if( fCutResults.begin(), fCutResults.end(), CheckCutResultName(name) );
    }

    template< typename XCutType >
    inline KTCutStatus::CutResultsCIt KTCutStatus::FindCutResultC() const
    {
        return std::find_if( fCutResults.cbegin(), fCutResults.cend(), CheckCutResultType< XCutType > );
    }

    inline KTCutStatus::CutResultsCIt KTCutStatus::FindCutResultC( const std::string& name ) const
    {
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

    template< typename XCutType >
    inline bool KTCutStatus::HasCutResult() const
    {
        return FindCutResultC< XCutType >() != std::end(fCutResults);
    }

    inline bool KTCutStatus::HasCutResult( const std::string& name) const
    {
        return FindCutResultC(name) != std::end(fCutResults);
    }

    template< typename XCutType >
    bool KTCutStatus::GetCutState() const
    {
        auto cutIt = FindCutResult< XCutType >();
        if (cutIt != fCutResults.cend())
        {
            return cutIt->fState;
        }
        return false;
    }

    bool KTCutStatus::GetCutState( const std::string& name ) const
    {
        CutResultsCIt cutIt = FindCutResultC(name);
        if (cutIt != fCutResults.cend())
        {
            return (*cutIt)->fState;
        }
        return false;
    }

    template< typename XCutType >
    inline void KTCutStatus::RemoveCutResult(bool doUpdateStatus)
    {
        auto cutIt = FindCutResult< XCutType >();
        if (cutIt != fCutResults.end()) fCutResults.erase(cutIt);
        if (doUpdateStatus) UpdateStatus();
        return;
    }

    inline void KTCutStatus::RemoveCutResult(const std::string& name, bool doUpdateStatus)
    {
        auto cutIt = FindCutResult(name);
        if (cutIt != fCutResults.end()) fCutResults.erase(cutIt);
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
