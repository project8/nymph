/*
 * KTCutResult.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef KTCUTRESULT_HH_
#define KTCUTRESULT_HH_

//#include "KTExtensibleStruct.hh"

//#include "KTMemberVariable.hh"

#include <string>

namespace Nymph
{
    struct KTCutResult
    {
        bool fState;
        KTCutResult( bool state ) { fState = state; }
        virtual ~KTCutResult() {}
        virtual const std::string& Name() const = 0;
    };

    template< class XCutType >
    struct KTTypedCutResult : KTCutResult
    {
        KTTypedCutResult( bool state ) : KTCutResult( state ) {}
        virtual ~KTTypedCutResult() {}
        virtual const std::string& Name() const
        {
            return XCutType::GetName();
        }
    };

    template< class XCutType >
    bool CheckCutResultType( std::shared_ptr< KTCutResult > result )
    {
        return std::dynamic_pointer_cast< KTTypedCutResult< XCutType > >( result );

    }

    struct CheckCutResultName
    {
        std::string fName;
        CheckCutResultName( const std::string& name ) : fName( name ) {}
        bool operator()( std::shared_ptr< KTCutResult > result )
        {
            return result->Name() == fName;
        }
    };

} /* namespace Nymph */

#endif /* KTCUTRESULT_HH_ */
