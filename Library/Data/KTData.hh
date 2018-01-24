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

#include "KTLogger.hh"

#include <boost/serialization/string.hpp>

#include <string>
/*
namespace boost
{
    namespace archive
    {
        class polymorphic_iarchive;
        class polymorphic_oarchive;
    }
}
*/
namespace bs  = boost::serialization;

namespace Nymph
{
    LOGGER( logdata_hh, "KTData.hh" );

    class KTData
    {
        public:
            KTData();
            virtual ~KTData();

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version )
            {
                std::cout << "### serialize for KTData" << std::endl;
                return;
            }
    };

    class KTDataRider
    {
        public:
            KTDataRider();
            virtual ~KTDataRider();

            MEMBERVARIABLE_REF( std::string, Name );

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version )
            {
                std::cout << "### serialize for KTDataRider" << std::endl;
                ar & fName;
                return;
            }
    };

    template< class XDerivedType >
    class KTExtensibleDataRider : public KTExtensibleStruct< XDerivedType, KTDataRider >
    {
        public:
            KTExtensibleDataRider() = delete;
            KTExtensibleDataRider( const std::string& name ) { KTDataRider::fName = name; }
            virtual ~KTExtensibleDataRider() {}

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version )
            {
                std::cout << "### serialize for KTExtensibleDataRider" << std::endl;
                ar & bs::base_object< KTExtensibleStruct< XDerivedType, KTDataRider > >( *this );
                return;
            }
    };

#define DEFINE_EXT_DATA_2( ex_data_class_name, data_class_name, label ) \
        class ex_data_class_name : public data_class_name, public KTExtensibleDataRider< ex_data_class_name > \
        { \
            public: \
                ex_data_class_name() : data_class_name(), KTExtensibleDataRider< ex_data_class_name >( label ) {} \
                virtual ~ex_data_class_name() {} \
            private: \
                friend class bs::access; \
                template< class Archive > \
                void serialize( Archive& ar, const unsigned version ) \
                { \
                    /*KTWARN( logdata_hh, "In extensible data " );*/ \
                    std::cout << "### serialize for ex_data_class_name" << std::endl; \
                    ar & bs::base_object< data_class_name >( *this ); \
                    ar & bs::base_object< KTExtensibleDataRider< ex_data_class_name > >( *this ); \
                    return; \
                } \
        };

#define DEFINE_EXT_DATA( data_class_name, label ) DEFINE_EXT_DATA_2( PASTE(data_class_name, Ext), data_class_name, label )

} /* namespace Nymph */
#endif /* KTDATA_HH_ */
