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
            KTData();
            virtual ~KTData();
    };

    class KTDataRider
    {
        public:
            KTDataRider();
            virtual ~KTDataRider();

            MEMBERVARIABLE_REF( std::string, Name );
    };

    template< class XDerivedType >
    class KTExtensibleDataRider : public KTExtensibleStruct< XDerivedType, KTDataRider >
    {
        public:
            KTExtensibleDataRider() = delete;
            KTExtensibleDataRider( const std::string& name ) { KTDataRider::fName = name; }
            virtual ~KTExtensibleDataRider() {}

    };

#define DEFINE_EXT_DATA_2( ex_data_class_name, data_class_name, label ) \
        class ex_data_class_name : public data_class_name, public KTExtensibleDataRider< ex_data_class_name > \
        { \
            public: \
                ex_data_class_name() : data_class_name(), KTExtensibleDataRider< ex_data_class_name >( label ) {} \
                virtual ~ex_data_class_name() {} \
        };

#define DEFINE_EXT_DATA( data_class_name, label ) DEFINE_EXT_DATA_2( PASTE(data_class_name, Ext), data_class_name, label )

} /* namespace Nymph */
#endif /* KTDATA_HH_ */
