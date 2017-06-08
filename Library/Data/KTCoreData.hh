/*
 * KTCoreData.hh
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#ifndef KTCOREDATA_HH_
#define KTCOREDATA_HH_

#include "KTData.hh"

#include "KTCutStatus.hh"
#include "KTMemberVariable.hh"

#include <memory>
#include <string>

namespace Nymph
{

    class KTCoreData : public KTData
    {
        public:
            KTCoreData();
            KTCoreData( const KTCoreData& orig );
            KTCoreData( KTCoreData&& orig );
            virtual ~KTCoreData();

            MEMBERVARIABLE( unsigned, Counter );
            MEMBERVARIABLE( bool, LastData );

            MEMBERVARIABLE_REF( KTCutStatus, CutStatus );

    };

    DEFINE_EXT_DATA( KTCoreData )

    typedef std::shared_ptr< KTCoreData > KTDataHandle;

} /* namespace Nymph */
#endif /* KTCOREDATA_HH_ */
