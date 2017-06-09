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
#include "KTLogger.hh"
#include "KTMemberVariable.hh"

#include <memory>
#include <string>

namespace Nymph
{
    LOGGER( cdlog_h, "KTCoreData.hh" )

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


    // Define KTDataHandle and a convenience function to create new handles
    typedef std::shared_ptr< KTCoreDataExt > KTDataHandle;
    KTDataHandle CreateNewDataHandle();


    // Recursive helper structs for determining if data objects are present

    // End of the recursion; this will only be used if NoOtherDataTypes has nothing in it
    template< class... NoOtherDataTypes >
    struct DataPresentHelper
    {
        static bool DataPresent( KTDataHandle );
    };

    template< class DataType, class... OtherDataTypes >
    struct DataPresentHelper< DataType, OtherDataTypes... >
    {
        static bool DataPresent( KTDataHandle data );
    };

    template< class... NoOtherDataTypes >
    bool DataPresentHelper< NoOtherDataTypes... >::DataPresent( KTDataHandle )
    {
        return true;
    }

    template< class DataType, class... OtherDataTypes >
    bool DataPresentHelper< DataType, OtherDataTypes... >::DataPresent( KTDataHandle data )
    {
        if( ! data->Has< DataType >() )
        {
            KTERROR( cdlog_h, "Data not found with type <" << typeid( DataType ).name() << ">" );
            return false;
        }
        return DataPresentHelper< OtherDataTypes... >::DataPresent( data );
    }


} /* namespace Nymph */
#endif /* KTCOREDATA_HH_ */
