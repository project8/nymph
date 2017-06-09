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
            virtual ~KTCoreData();

            MEMBERVARIABLE( unsigned, Counter );
            MEMBERVARIABLE( bool, LastData );

            MEMBERVARIABLE_REF( KTCutStatus, CutStatus );

    };

    DEFINE_EXT_DATA( KTCoreData, "core" )

    typedef std::shared_ptr< KTCoreDataExt > KTDataHandle;
    KTDataHandle CreateNewDataHandle();

} /* namespace Nymph */
#endif /* KTCOREDATA_HH_ */
