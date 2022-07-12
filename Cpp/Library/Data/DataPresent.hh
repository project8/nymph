 

#ifndef NYMPH_DATAPRESENT_HH_
#define NYMPH_DATAPRESENT_HH_

#include "DataFrame.hh"

#include "logger.hh"
 
namespace Nymph
{
    LOGGER( dplog_h, "DataPresent" );
 
    // Recursive helper structs for determining if data objects are present

    // End of the recursion; this will only be used if NoOtherDataTypes has nothing in it
    template< class... NoOtherDataTypes >
    struct DataPresentHelper
    {
        static bool DataPresent( const DataHandle& );
    };

    template< class XDataType, class... XOtherDataTypes >
    struct DataPresentHelper< XDataType, XOtherDataTypes... >
    {
        static bool DataPresent( const DataHandle& handle );
    };

    template< class... NoOtherDataTypes >
    bool DataPresentHelper< NoOtherDataTypes... >::DataPresent( const DataHandle& )
    {
        return true;
    }

    template< class XDataType, class... XOtherDataTypes >
    bool DataPresentHelper< XDataType, XOtherDataTypes... >::DataPresent( const DataHandle& handle )
    {
        LINFO(dplog_h, "TEST IN CATCH IN APPLY FUNCTION" );
        if( ! handle->Has< XDataType >() )
        {
            LERROR( dplog_h, "Data not found with type <" << scarab::type(XDataType()) << ">" );
            return false;
        }
        return DataPresentHelper< XOtherDataTypes... >::DataPresent( handle );
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAPRESENT_HH_ */
