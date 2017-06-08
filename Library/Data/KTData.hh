/*
 * KTData.hh
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#ifndef KTDATA_HH_
#define KTDATA_HH_

#include "KTExtensibleStruct.hh"

#include "KTMemberVariable.hh"

#include <string>

namespace Nymph
{

    class KTData
    {
        public:
            KTData() = delete;
            KTData( const std::string& name );
            KTData( const KTData& orig );
            KTData( KTData&& orig );
            virtual ~KTData();

            MEMBERVARIABLE_REF( std::string, Name );
    };

    class KTDataRider
    {
        public:
            KTDataRider() {}
            virtual ~KTDataRider() {}

    };

    template< class XDerivedType >
    class KTExtensibleDataRider : public KTExtensibleStruct< XDerivedType, KTDataRider >
    {
        public:
            KTExtensibleDataRider() {}
            virtual ~KTExtensibleDataRider() {}

    };

#define DEFINE_EXT_DATA_2( ex_data_class_name, data_class_name ) \
        class ex_data_class_name : public data_class_name, public KTExtensibleDataRider< ex_data_class_name > \
        { \
            public: \
                ex_data_class_name() : data_class_name(), KTExtensibleDataRider< ex_data_class_name >() {} \
                ex_data_class_name( const ex_data_class_name& orig ) : data_class_name( orig ), KTExtensibleDataRider< ex_data_class_name >( orig ) {} \
                ex_data_class_name( ex_data_class_name&& orig ) : data_class_name( orig ), KTExtensibleDataRider< ex_data_class_name >( orig ) {} \
                virtual ~ex_data_class_name() {} \
        };

#define DEFINE_EXT_DATA( data_class_name ) DEFINE_EXT_DATA_2( PASTE(data_class_name, Ext), data_class_name )

} /* namespace Nymph */
#endif /* KTDATA_HH_ */
