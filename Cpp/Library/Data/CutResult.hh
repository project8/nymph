/*
 * CutResult.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef CUTRESULT_HH_
#define CUTRESULT_HH_

#include <string>
#include <map>

namespace Nymph
{
    typedef std::map< std::string, bool > CutResult;
    typedef CutResult::value_type fCutVal;
    // This might be enough. gives key fName to return bool of if cut
    // <map>::value_type is just the pair value type def used to initiate the map
    // so give fCutVal(<string> fName,<bool> value) and then insert into CutResult

/*    struct CutResult
    {
        std::string fName;
        bool fState;
        bool fAssigned;
        CutResult() : fName(), fState( false ), fAssigned( false ) {}
        CutResult( const std::string& name, bool state ) : fName( name ), fState( state ), fAssigned( true ) {}
    };

    struct CheckCutResultName
    {
        std::string fName;
        CheckCutResultName( const std::string& name ) : fName( name ) {}
        bool operator()( const CutResult& result )
        {
            return result.fName == fName;
        }
    };
*/
} /* namespace Nymph */

#endif /* CUTRESULT_HH_ */
