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

#include "cereal/types/polymorphic.hpp" // required to avoid compile error
#include "cereal/types/base_class.hpp" // required for base-class specification
#include "cereal/access.hpp" // required for private member access

#include <string>

namespace Nymph
{

    class KTData
    {
        public:
            KTData();
            virtual ~KTData();

            template< class Archive >
            void serialize( Archive& ar )
            {
                std::cout << "### serialize for KTData" << std::endl;
            }
    };

    class KTDataRider
    {
        public:
            KTDataRider();
            KTDataRider( const KTDataRider& orig );
            virtual ~KTDataRider();

            MEMBERVARIABLE_REF( std::string, Name );

        private:
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
    };

    template< class Archive >
    void KTDataRider::serialize( Archive& ar )
    {
            std::cout << "### serialize for KTDataRider" << std::endl;
            ar( fName );
    }

    template< class XDerivedType >
    class KTExtensibleDataRider : public KTExtensibleStruct< XDerivedType, KTDataRider >
    {
        public:
            KTExtensibleDataRider() = delete;
            KTExtensibleDataRider( const std::string& name ) { KTDataRider::fName = name; }
            KTExtensibleDataRider( const KTExtensibleDataRider< XDerivedType >& orig ) : KTExtensibleStruct< XDerivedType, KTDataRider >( *this ) {}
            virtual ~KTExtensibleDataRider() {}

/*
            template< class Archive >
            void serialize( Archive& ar )
            {
                std::cout << "### serialize for KTExtensibleStruct< XDerivedType, KTDataRider >" << std::endl;
                ar( cereal::base_class< KTExtensibleStruct< XDerivedType, KTDataRider > >( this ) );
            }
*/
    };

#define DEFINE_EXT_DATA_2( ex_data_class_name, data_class_name, label ) \
        class ex_data_class_name : public data_class_name, public KTExtensibleDataRider< ex_data_class_name > \
        { \
            public: \
                ex_data_class_name() : data_class_name(), KTExtensibleDataRider< ex_data_class_name >( label ) {} \
                ex_data_class_name( const ex_data_class_name& orig ) : data_class_name( *this ), KTExtensibleDataRider< ex_data_class_name >( *this ) {} \
                virtual ~ex_data_class_name() {} \
                \
                std::shared_ptr< KTExtensibleStructCore< KTDataRider > > Clone() const \
                { \
                    std::shared_ptr< ex_data_class_name > copy = std::make_shared< ex_data_class_name >( *this ); \
                    return std::static_pointer_cast< KTExtensibleStructCore< KTDataRider >, ex_data_class_name >( copy ); \
                } \
        };

/*
                template< class Archive > \
                void serialize( Archive& ar ) \
                { \
                    std::cout << "### serialize for ex_data_class_name" << std::endl;
                    ar( cereal::base_class< data_class_name >( this ), cereal::base_class< KTExtensibleDataRider< ex_data_class_name > >( this ) ); \
                } \
*/

#define DEFINE_EXT_DATA( data_class_name, label ) DEFINE_EXT_DATA_2( PASTE(data_class_name, Ext), data_class_name, label )

} /* namespace Nymph */
#endif /* KTDATA_HH_ */
