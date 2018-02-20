/*
 * KTCutResult.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef KTCUTRESULT_HH_
#define KTCUTRESULT_HH_

#include <string>

namespace Nymph
{
    struct KTCutResult
    {
        std::string fName;
        bool fState;
        bool fAssigned;
        KTCutResult() : fName(), fState( false ), fAssigned( false ) {}
        KTCutResult( const std::string& name, bool state ) : fName( name ), fState( state ), fAssigned( true ) {}
    };

    struct CheckCutResultName
    {
        std::string fName;
        CheckCutResultName( const std::string& name ) : fName( name ) {}
        bool operator()( const KTCutResult& result )
        {
            return result.fName == fName;
        }
    };

} /* namespace Nymph */

#endif /* KTCUTRESULT_HH_ */
