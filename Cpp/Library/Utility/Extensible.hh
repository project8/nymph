/*
 * Extensible.hh
 *
 *  Created on: Feb 11, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_EXTENSIBLE_HH_
#define NYMPH_EXTENSIBLE_HH_

#include "Exception.hh"

//#include <cereal/types/polymorphic.hpp>

//#include <memory>

namespace Nymph
{

    template< class XBaseType >
    class ExtensibleCore : public XBaseType, public std::enable_shared_from_this< ExtensibleCore< XBaseType > >
    {
        public:
            typedef ExtensibleCore< XBaseType > ExtCoreType;
            typedef std::shared_ptr< ExtCoreType > BasePtrType;

        private:
            typedef std::enable_shared_from_this< ExtensibleCore< XBaseType > > ESFTBaseType;

        public:
            ExtensibleCore();
            ExtensibleCore( const ExtCoreType& orig ) = delete; // deleted because copy of ExtensibleCore is done using operator=()
            virtual ~ExtensibleCore();

            /// Copies extended object
            ExtCoreType& operator=( const ExtCoreType& orig );

            /// Copies the current object and only extended fields already present
            virtual ExtCoreType& Pull( const ExtCoreType& orig ) = 0; // implemented in Extensible because it requires knowing the extended field type

            /// Clones the extended object
            virtual BasePtrType Clone() const = 0; // implemented in Extensible because it requires knowing the extended field type

            /// Returns the size of the extended object, including this field and any extended below it
            unsigned size() const;

            /// Returns the object of the requested type or creates a new one if it's not present
            template< class XRequestedType >
            XRequestedType& Of();

            /// Returns the object of the requested type or throws KTException if it's not present
            template< class XRequestedType >
            const XRequestedType& Of() const;

            /// Returns a pointer to the object of the requested type or creates a new one if it's not present
            template< class XRequestedType >
            std::shared_ptr< XRequestedType > Share();

            /// Returns a pointer to the object of the requested type or throws KTException if it's not present
            template< class XRequestedType >
            const std::shared_ptr< XRequestedType > Share() const;

            /// Checks whether the requested type is present
            template< class XRequestedType >
            bool Has() const;

            /// Extracts the data object of the requested type if it's present; returns an empty pointer if not
            /// Note that if you detatch the first field without holding a pointer to any other extended fields, those fields will be destroyed.
            template< class XRequestedType >
            std::shared_ptr< XRequestedType > Detatch();

            /// Returns a pointer to the next extended field
            BasePtrType Next();

        protected:
            template< class XRequestedType >
            std::shared_ptr< XRequestedType > _Detatch( BasePtrType prev = BasePtrType() );

            BasePtrType fNext;

            bool fDisableExtendedCopy; // internal variable used to determine whether operator= copies extended fields

        private:
//----------RemovedSerial-------------
/*
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
*/
    };

    template< class XInstanceType, class XBaseType >
    class Extensible : public ExtensibleCore< XBaseType >
    {
        public:
            typedef Extensible< XInstanceType, XBaseType > ExtType;
            typedef ExtensibleCore< XBaseType > ExtCoreType;
            typedef std::shared_ptr< ExtensibleCore< XBaseType > > BasePtrType;

        public:
            Extensible();
            Extensible( const ExtType& orig );
            virtual ~Extensible();

            /// Copies the extended object
            ExtType& operator=( const ExtType& orig );

            /// Copies the current object and only extended fields already present
            virtual ExtCoreType& Pull( const ExtCoreType& orig );

            /// Clones the extended object
            virtual BasePtrType Clone() const;

        private:
//----------RemovedSerial-------------
/*  
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
*/
    };



    //**************
    // ExtensibleCore implementation
    //**************

    template< class XBaseType >
    ExtensibleCore< XBaseType >::ExtensibleCore() :
            XBaseType(),
            fNext(),
            fDisableExtendedCopy( false )
    {
    }

    template< class XBaseType >
    ExtensibleCore< XBaseType >::~ExtensibleCore()
    {
    }

    template< class XBaseType >
    ExtensibleCore< XBaseType >& ExtensibleCore< XBaseType >::operator=( const ExtensibleCore< XBaseType >& orig )
    {
        // must not call virtual functions on this because it's used in the copy constructor

        if( this == &orig ) return *this;

        fDisableExtendedCopy = false;

        // remove any extended fields
        fNext.reset();

        // copy extended fields
        // only do this if orig.fDisableExtendedCopy is false
        if( ! orig.fDisableExtendedCopy && orig.fNext )
        {
            // duplicate extended fields with Clone()
            fNext = orig.fNext->Clone();  // this's fNext points to the copy of orig's fNext
        }

        // duplicate base class
        this->XBaseType::operator=( *this );

        return *this;
    }

    template< class XBaseType >
    unsigned ExtensibleCore< XBaseType >::size() const
    {
        return fNext.operator bool() ? 1 + fNext->size() : 1;
    }

    template< class XBaseType >
    template< class XRequestedType >
    XRequestedType& ExtensibleCore< XBaseType >::Of()
    {
        XRequestedType* requested = dynamic_cast< XRequestedType* >( this );
        if( requested != nullptr )
        {
            return *requested;
        }

        if( ! fNext )
        {
            std::shared_ptr< XRequestedType > requestedShared = std::make_shared< XRequestedType >();
            fNext = requestedShared;
            return *requestedShared;
        }
        return fNext->template Of< XRequestedType >();
    }

    template< class XBaseType >
    template< class XRequestedType >
    const XRequestedType& ExtensibleCore< XBaseType >::Of() const
    {
        const XRequestedType* requested = dynamic_cast< const XRequestedType* >( this );
        if( requested != nullptr )
        {
            return *requested;
        }

        if( ! fNext )
        {
            throw Exception() << "Cannot add to a const extensible object";
        }
        return fNext->template Of< XRequestedType >();
    }

    template< class XBaseType >
    template< class XRequestedType >
    std::shared_ptr< XRequestedType > ExtensibleCore< XBaseType >::Share()
    {
        std::shared_ptr< XRequestedType > requested = std::dynamic_pointer_cast< XRequestedType >( ESFTBaseType::shared_from_this() );
        if( requested )
        {
            std::cout << "type match" << std::endl;
            return requested;
        }

        if( ! fNext )
        {
            requested = std::make_shared< XRequestedType >();
            fNext = requested;
            return requested;
        }
        return fNext->template Share< XRequestedType >();
    }

    template< class XBaseType >
    template< class XRequestedType >
    const std::shared_ptr< XRequestedType > ExtensibleCore< XBaseType >::Share() const
    {
        const std::shared_ptr< XRequestedType > requested = std::dynamic_pointer_cast< const XRequestedType >( ESFTBaseType::shared_from_this() );
        if( requested )
        {
            return requested;
        }

        if( ! fNext )
        {
            throw Exception() << "Cannot add to a const extensible object";
        }
        return fNext->template Share< XRequestedType >();
    }

    template< class XBaseType >
    template< class XRequestedType >
    bool ExtensibleCore< XBaseType >::Has() const
    {
        if( dynamic_cast< const XRequestedType* >( this ) )
        {
            return true;
        }
        if( fNext )
        {
            return fNext->template Has< XRequestedType >();
        }
        return false;
    }

    template< class XBaseType >
    template< class XRequestedType >
    std::shared_ptr< XRequestedType > ExtensibleCore< XBaseType >::Detatch()
    {
        return _Detatch< XRequestedType >();
    }

    template< class XBaseType >
    std::shared_ptr< ExtensibleCore< XBaseType > > ExtensibleCore< XBaseType >::Next()
    {
        return fNext;
    }

    template< class XBaseType >
    template< class XRequestedType >
    std::shared_ptr< XRequestedType > ExtensibleCore< XBaseType >::_Detatch( ExtensibleCore< XBaseType >::BasePtrType prev )
    {
        if( dynamic_cast< XRequestedType* >( this ) )
        {
            // create a shared pointer to this here, so that when we reset prev.fNext (if it exists), that won't be the last reference to this
            std::shared_ptr< XRequestedType > detatched = std::static_pointer_cast< XRequestedType >( ESFTBaseType::shared_from_this() );
            if( prev )
            {
                // link prev and fNext if fNext exists; otherwise resets prev.fNext
                prev->fNext = this->fNext;
            }
            fNext.reset();
            return detatched;
        }

        if( fNext )
        {
            return fNext->template _Detatch< XRequestedType >( ESFTBaseType::shared_from_this() );
        }

        return std::shared_ptr< XRequestedType >();
    }

/*
    template< class XBaseType >
    template< class Archive >
    void KTExtensibleCore< XBaseType >::serialize( Archive& ar )
    {
        std::cout << "### serialize for " << typeid(KTExtensibleCore< XBaseType >).name() << std::endl;
        ar( cereal::base_class< XBaseType >( this ), fNext );
    }
*/

    //**************
    // Extensible implementation
    //**************

    template< class XInstanceType, class XBaseType >
    Extensible< XInstanceType, XBaseType >::Extensible() :
            ExtensibleCore< XBaseType >()
    {}

    template< class XInstanceType, class XBaseType >
    Extensible< XInstanceType, XBaseType >::Extensible( const Extensible< XInstanceType, XBaseType >& orig ) :
            ExtensibleCore< XBaseType >() // copy of ExtensibleCore is done from operator=()
    {
        *this = orig;
    }

    template< class XInstanceType, class XBaseType >
    Extensible< XInstanceType, XBaseType >::~Extensible()
    {}

    template< class XInstanceType, class XBaseType >
    Extensible< XInstanceType, XBaseType >& Extensible< XInstanceType, XBaseType >::operator=( const Extensible< XInstanceType, XBaseType >& orig )
    {
        // must not call any virtual functions on `this` because it's called from the copy constructor

        if( this == &orig ) return *this;

        this->ExtensibleCore< XBaseType >::operator=( orig );

        return *this;
    }

    template< class XInstanceType, class XBaseType >
    ExtensibleCore< XBaseType >& Extensible< XInstanceType, XBaseType >::Pull( const ExtensibleCore< XBaseType >& orig )
    {
        // copies information out from orig based on this's instance type and any extended fields present

        if( this == &orig ) return *this;

        this->fDisableExtendedCopy = true; // we're going to use operator=, so we need to disable copying of extended fields to only get the instance type
        XInstanceType* instance = static_cast< XInstanceType* >( this ); // guaranteed to be ok by CRTP
        // orig is const, so Of() will throw an exception if XInstanceType is not present
        // in that case, make a copy of a new object
        try
        {
            instance->operator=( orig.template Of< XInstanceType >() );
        }
        catch( Exception& )
        {
            instance->operator=( XInstanceType() );
        }
        this->fDisableExtendedCopy = false;

        // continue the pull process for any extended fields present in this
        if( this->fNext )
        {
            this->fNext->Pull( orig );
        }

        return *this;
    }

    template< class XInstanceType, class XBaseType >
    std::shared_ptr< ExtensibleCore< XBaseType > > Extensible< XInstanceType, XBaseType >::Clone() const
    {
        std::shared_ptr< XInstanceType > instancePtr = std::make_shared< XInstanceType >();
        instancePtr->Pull(*this); // extracts just XInstanceType from this because instancePtr doesn't have any extended fields

        // if there are extended fields, clone them
        if( this->fNext )
        {
            instancePtr->fNext = this->fNext->Clone(); // instancePtr's fNext is a clone of this's fNext
        }

        return instancePtr;
    }

/*
    template< class XInstanceType, class XBaseType >
    template< class Archive >
    void KTExtensible< XInstanceType, XBaseType >::serialize( Archive& ar )
    {
        std::cout << "### serialize for " << typeid(KTExtensible< XInstanceType, XBaseType >).name() << std::endl;
        ar( cereal::base_class< ExtCoreType >( this ) );
        return;
    }
*/

} /* namespace Nymph */

#endif /* NYMPH_EXTENSIBLE_HH_ */
