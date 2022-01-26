 

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
        static bool DataPresent( const DataFrame& );
    };

    template< class XDataType, class... XOtherDataTypes >
    struct DataPresentHelper< XDataType, XOtherDataTypes... >
    {
        static bool DataPresent( const DataFrame& frame );
    };

    template< class... NoOtherDataTypes >
    bool DataPresentHelper< NoOtherDataTypes... >::DataPresent( const DataFrame& )
    {
        return true;
    }

    template< class XDataType, class... XOtherDataTypes >
    bool DataPresentHelper< XDataType, XOtherDataTypes... >::DataPresent( const DataFrame& frame )
    {
        if( ! frame.Has< XDataType >() )
        {
            LERROR( dplog_h, "Data not found with type <" << scarab::type(XDataType()) << ">" );
            return false;
        }
        return DataPresentHelper< XOtherDataTypes... >::DataPresent( frame );
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAPRESENT_HH_ */
