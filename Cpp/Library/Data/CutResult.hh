/*
 * CutResult.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef CUTRESULT_HH_
#define CUTRESULT_HH_

#include <string>

namespace Nymph
{
    struct CutResult
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

} /* namespace Nymph */

#endif /* CUTRESULT_HH_ */
