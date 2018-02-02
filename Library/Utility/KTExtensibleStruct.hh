/*
 * KTExtensibleStruct.hh
 *
 *  Created on: Feb 14, 2013
 *      Author: nsoblath
 *      Original Author: Sanshiro Enomoto <sanshiro@uw.edu> (KDExternalStruct, from the KATRIN experiment)
 */

#ifndef KTEXTENSIBLESTRUCT_HH_
#define KTEXTENSIBLESTRUCT_HH_

#include <cereal/types/polymorphic.hpp>

#include <memory>

namespace Nymph
{

    // *) operator= reallocates extended fields: to avoid this, use Pull().

    /**
     *
     */

    template< class XBaseType >
    struct KTExtensibleStructCore : public XBaseType
    {
        public:
            typedef std::shared_ptr< XBaseType > BasePtr;

        public:
            /// Default constructor
            KTExtensibleStructCore();
            /// Copy constructor; duplicates the extended object
            KTExtensibleStructCore( const KTExtensibleStructCore& );
            virtual ~KTExtensibleStructCore();
            /// Duplicates the extended object
            KTExtensibleStructCore& operator=( const KTExtensibleStructCore& );
            /// Removes extended fields
            virtual void Clear();
            /// Returns a reference to the object of type XStructType; creates that object if it doesn't exist
            template< class XStructType >
            inline XStructType& Of();
            /// Returns a const reference to the object of type XStructType; creates that object if it doesn't exist
            template< class XStructType >
            inline const XStructType& Of() const;
            /// Returns true if XStructType is or is below this object
            template< class XStructType >
            inline bool Has() const;
            /// Extracts object of type XStructType
            template< class XStructType >
            inline std::shared_ptr< XStructType > Detatch();
            /// Duplicates the extended object
            virtual BasePtr Clone() const = 0;
            /// Duplicates object only
            virtual void Pull( const KTExtensibleStructCore< XBaseType >& object ) = 0;
            /// Returns the pointer to the next field
            BasePtr Next() const;
            /// Returns the pointer to the previous field
            BasePtr Prev() const;
            /// Returns the pointer to the last field
            BasePtr Last() const;
            /// Returns the pointer to the first field
            BasePtr First() const;
        protected:
            void SetPrevPtrInNext();
            mutable BasePtr fNext;
            mutable BasePtr fPrev;

        private:
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
    };



    template< class XInstanceType, class XBaseType >
    struct KTExtensibleStruct : KTExtensibleStructCore< XBaseType >
    {
        public:
            typedef typename KTExtensibleStructCore< XBaseType >::BasePtr BasePtr;

        public:
            /// Default constructor
            KTExtensibleStruct();
            /// Copy constructor; duplicates the extended object
            KTExtensibleStruct( const KTExtensibleStruct& object );
            virtual ~KTExtensibleStruct();
            /// Duplicates the extended object
            KTExtensibleStruct& operator=( const KTExtensibleStruct& object );
            /// Duplicates the extended object
            virtual BasePtr Clone() const;
            /// Duplicates object only
            virtual void Pull( const KTExtensibleStructCore< XBaseType >& object );
            void SetIsCopyDisabled( bool flag );
        private:
            bool fIsCopyDisabled;
            friend class cereal::access;

            template< class Archive >
            void serialize( Archive& ar );
    };



    template<class XBaseType>
    KTExtensibleStructCore<XBaseType>::KTExtensibleStructCore(void) :
            fNext(),
            fPrev()
    {
    }

    template<class XBaseType>
    KTExtensibleStructCore<XBaseType>::KTExtensibleStructCore( const KTExtensibleStructCore& ) :
            fNext(),
            fPrev()
    {
    }

    template<class XBaseType>
    KTExtensibleStructCore<XBaseType>::~KTExtensibleStructCore()
    {
        fNext.reset();
    }

    template<class XBaseType>
    KTExtensibleStructCore<XBaseType>& KTExtensibleStructCore<XBaseType>::operator=( const KTExtensibleStructCore& )
    {
        fNext.reset();
        return *this;
    }

    template<class XBaseType>
    void KTExtensibleStructCore<XBaseType>::Clear()
    {
        fNext.reset();
    }

    template<class XBaseType>
    template<class XStructType>
    inline XStructType& KTExtensibleStructCore<XBaseType>::Of()
    {
        XStructType* target = dynamic_cast<XStructType*>(this);
        if (target)
        {
            return *target;
        }

        if (! fNext)
        {
            fNext = std::make_shared< XStructType >();
            fNext->fPrev.reset( this );
        }

        return fNext->template Of<XStructType>();
    }

    template<class XBaseType>
    template<class XStructType>
    inline const XStructType& KTExtensibleStructCore<XBaseType>::Of() const
    {
        const XStructType* target = dynamic_cast<const XStructType*>(this);
        if (target)
        {
            return *target;
        }

        if (! fNext)
        {
            fNext = std::make_shared< XStructType >();
            fNext->fPrev.reset( const_cast< KTExtensibleStructCore< XBaseType >* >(this) );
        }

        return fNext->template Of<XStructType>();
    }



    template<class XBaseType>
    template<class XStructType>
    inline bool KTExtensibleStructCore<XBaseType>::Has() const
    {
        if (dynamic_cast<const XStructType*>(this))
        {
            return true;
        }
        if (fNext)
        {
            return fNext->template Has<XStructType>();
        }

        return false;
    }



    template<class XBaseType>
    template<class XStructType>
    inline std::shared_ptr< XStructType > KTExtensibleStructCore<XBaseType>::Detatch()
    {
        if (! fNext)
        {
            return std::shared_ptr< XStructType >();
        }
        std::shared_ptr< XStructType > next = std::dynamic_pointer_cast< XStructType >(fNext);
        if (next)
        {
            if (next->fNext)
            {
                fNext = next->fNext;
                fNext->fPrev = this;
                next->fNext.reset();
            }
            next->fPrev.reset();
            return next;
        }
        return fNext->template Detatch<XStructType>();
    }


    template<class XBaseType>
    inline typename KTExtensibleStructCore<XBaseType>::BasePtr KTExtensibleStructCore<XBaseType>::Next() const
    {
        return fNext;
    }

    template<class XBaseType>
    inline typename KTExtensibleStructCore<XBaseType>::BasePtr KTExtensibleStructCore<XBaseType>::Prev() const
    {
        return fPrev;
    }

    template<class XBaseType>
    inline typename KTExtensibleStructCore<XBaseType>::BasePtr KTExtensibleStructCore<XBaseType>::Last() const
    {
        if (fNext) return this;
        return fNext->Last();
    }

    template<class XBaseType>
    inline typename KTExtensibleStructCore<XBaseType>::BasePtr KTExtensibleStructCore<XBaseType>::First() const
    {
        if (fPrev) return this;
        return fPrev->First();
    }

    template<class XBaseType>
    inline void KTExtensibleStructCore<XBaseType>::SetPrevPtrInNext()
    {
        fNext->fPrev.reset( this );
        return;
    }

    template< class XBaseType >
    template< class Archive >
    void KTExtensibleStructCore<XBaseType>::serialize( Archive& ar )
    {
        std::cout << "### serialize for " << typeid(XBaseType).name() << std::endl;
        ar( this );

        if( fNext == 0 ){ return; }
        
        // archive pointer ( fNext )?
    }



    template< class XInstanceType, class XBaseType >
    template< class Archive >
    void KTExtensibleStruct<XInstanceType, XBaseType>::serialize( Archive& ar )
    {
        std::cout << "### serialize for " << typeid(XBaseType).name() << std::endl;
        ar( cereal::base_class< KTExtensibleStructCore< XBaseType > >( this ), fIsCopyDisabled );

        return;
    }

    template<class XInstanceType, class XBaseType>
    KTExtensibleStruct<XInstanceType, XBaseType>::KTExtensibleStruct() :
            KTExtensibleStructCore< XBaseType >(),
            fIsCopyDisabled( false )
    {
    }

    template<class XInstanceType, class XBaseType>
    KTExtensibleStruct<XInstanceType, XBaseType>::~KTExtensibleStruct()
    {
        fIsCopyDisabled = false;
    }

    template<class XInstanceType, class XBaseType>
    KTExtensibleStruct<XInstanceType, XBaseType>::KTExtensibleStruct(const KTExtensibleStruct<XInstanceType, XBaseType>& object) :
            KTExtensibleStructCore<XBaseType>(object)
    {
        // should this check fIsCopyDisabled in object?
        fIsCopyDisabled = false;

        if (object.fNext)
        {
            this->fNext.reset( object.fNext->Clone() );
        }
    }

    template<class XInstanceType, class XBaseType>
    KTExtensibleStruct<XInstanceType, XBaseType>& KTExtensibleStruct<XInstanceType, XBaseType>::operator=(const KTExtensibleStruct<XInstanceType, XBaseType>& object)
    {
        // should this check fIsCopyDisabled in object?
        if ((&object == this) || fIsCopyDisabled)
        {
            return *this;
        }

        this->fNext.reset();

        if (object.fNext)
        {
            this->fNext = object.fNext->Clone();
            this->KTExtensibleStructCore< XBaseType >::SetPrevPtrInNext();
            //this->fNext->fPrev = this;
        }

        return *this;
    }

    template<class XInstanceType, class XBaseType>
    typename KTExtensibleStruct<XInstanceType, XBaseType>::BasePtr KTExtensibleStruct<XInstanceType, XBaseType>::Clone() const
    {
        // assume CRTP is used properly,
        // otherwise compiling fails here (intended behavior)
        BasePtr instance = std::make_shared< XBaseType >( XInstanceType(dynamic_cast<const XInstanceType&>(*this)) );
        if (this->fNext)
        {
            instance->fNext.reset( this->fNext->Clone() );
            instance->SetPrevPtrInNext();
            //instance->fNext->fPrev = instance->fNext;
        }
        return instance;
    }

    template<class XInstanceType, class XBaseType>
    void KTExtensibleStruct<XInstanceType, XBaseType>::Pull(const KTExtensibleStructCore<XBaseType>& object)
    {
        if (&object == this)
        {
            return;
        }

        fIsCopyDisabled = true;
        XInstanceType* instance = dynamic_cast<XInstanceType*>(this);
        if (object.template Has<XInstanceType>())
        {
            instance->operator=(object.template Of<XInstanceType>());
        }
        else
        {
            instance->operator=(XInstanceType());
        }
        fIsCopyDisabled = false;

        if (this->fNext)
        {
            this->fNext->Pull(object);
        }
    }

    template<class XInstanceType, class XBaseType>
    inline void KTExtensibleStruct<XInstanceType, XBaseType>::SetIsCopyDisabled(bool flag)
    {
        fIsCopyDisabled = flag;
        return;
    }

} /* namespace Nymph */
#endif /* KTEXTENSIBLESTRUCT_HH_ */
