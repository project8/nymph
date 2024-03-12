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

#include "Exception.hh"


namespace Nymph
{
    /*!
     @class CutStatus
     @author N. S. Oblath

     @brief Provides easy access to cut result information.

     @details
     The cut results are stored as a map with cut name as key, with value of bool of whether the cut is applied or not.

     CutStatus is typically used as a member variable of DataFrame.

     You can check if the data has been cut with the IsCut functions.
     - IsCut() returns true if any cut results are true;

     When specifying a cut, bools set to true specify cuts that are applied.

     With CutStatus you can interact with individual cut results in the following ways:
     - Check whether any cut results are set to true with IsCut(),
     - Get the number of cut results with size(),
     - Get the number of cut results with value true with NumCuts(),
     - Get a reference to the cut results with CutResults(), 
     - Add cut results to a dataframe with AssignCutResult(),
     - Remove a cut result with RemoveCutResult(),
     - Check to see if a particular cut result is present using HasCutResult(),
     - Get the value of a cut result with GetCutState(),
     - Set the value of a cut result with SetCutState(),
     - Get a vector of cut names for cuts with value==True with CutResultsPresent(), and
     - Output a text summary of applied cuts with the << operator,

     */

    class CutStatus
    {
        public:
            typedef std::map< std::string, bool > CutResults_t;
            typedef CutResults_t::iterator CutResultsIt;
            typedef CutResults_t::const_iterator CutResultsCIt;
    
        public: 
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
