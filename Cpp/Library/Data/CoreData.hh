/*
 * CoreData.hh
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#ifndef COREDATA_HH_
#define COREDATA_HH_

#include "Data.hh"
//#include "DataFrame.hh"

#include "CutStatus.hh"
#include "Logger.hh"
#include "MemberVariable.hh"

#include <memory>
#include <string>

namespace Nymph
{
    LOGGER( cdlog_h, "CoreData.hh" )

    class CoreData : public Data
    {
        public:
            CoreData();
            virtual ~CoreData();

            MEMVAR( unsigned, Counter );
            MEMVAR( bool, LastData );

            MEMVAR_REF( CutStatus, GetCutStatus );
	    //CutStatus fGetCutStatus;
        private:
//-------------RemovedSerial---------------
/*
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar )
            {
                std::cout << "### serialize for KTCoreData" << std::endl;
                ar( cereal::base_class< KTData >( this ), CEREAL_NVP(fCounter), CEREAL_NVP(fLastData), CEREAL_NVP(fCutStatus) );
            }
*/
    };

//    DEFINE_EXT_DATA( CoreData, "core" )

    class ExtCoreData : public CoreData
    {
        public:
            ExtCoreData();
            ExtCoreData(std::string Name);
            virtual ~ExtCoreData();

            MEMVAR_REF( std::string, Name );            
//            MEMVAR_REF( CutStatus, GetCutStatus );
//	    CutStatus fGetCutStatus;
    };

/*
    //THIS FUNCTIONALITY HAS BEEN MOVED TO DataFramework.hh
    // Define DataHandle and a convenience function to create new handles
    typedef std::shared_ptr< KTCoreDataExt > KTDataHandle;
    DataFrame::DataHandle CreateNewDataHandle();
*/

/*
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
*/

} /* namespace Nymph */
#endif /* COREDATA_HH_ */
