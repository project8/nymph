/*
 * KTThreadReference.hh
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#ifndef KTTHREADREFERENCE_HH_
#define KTTHREADREFERENCE_HH_

#include "KTCoreData.hh"

#include <boost/thread/future.hpp>

#include <memory>

namespace Nymph
{
    typedef boost::promise< KTDataHandle > KTDataPtrReturn;

    class KTThreadReferenceBase
    {
        protected:
            typedef boost::unique_lock< boost::mutex > boost_unique_lock;

        public:
            KTThreadReferenceBase();
            KTThreadReferenceBase( const KTThreadReferenceBase& ) = delete;
            KTThreadReferenceBase( KTThreadReferenceBase&& orig );

            KTThreadReferenceBase& operator=( const KTThreadReferenceBase& ) = delete;
            KTThreadReferenceBase& operator=( KTThreadReferenceBase&& );

        public:
            //**************************
            // for use within the thread
            //**************************

            void SetReturnException( boost::exception_ptr excPtr );

        public:
            //******************************
            // for use outside of the thread
            //******************************

            void SetInitiateBreakFunc( const std::function< void() >& initBreakFunc );
            void SetWaitForContinueFunc( const std::function< void( boost_unique_lock& ) >& waitForContFunc );

        public:
            MEMBERVARIABLE_REF( std::string, Name );
            MEMBERVARIABLE( bool, BreakFlag );
            MEMBERVARIABLE( bool, Canceled );
            MEMBERVARIABLE_REF( boost::mutex, Mutex );

        private:
            std::function< void() > fInitiateBreakFunc;
            std::function< void( boost_unique_lock& ) > fWaitForContinueFunc;
    };

    template< typename... XArgs >
    class KTThreadReference : public KTThreadReferenceBase
    {
        private:
            typedef KTThreadReferenceBase::boost_unique_lock boost_unique_lock;

        public:
            KTThreadReference();
            KTThreadReference( const KTThreadReference& ) = delete;
            KTThreadReference( KTThreadReference&& orig );

            KTThreadReference& operator=( const KTThreadReference& ) = delete;
            KTThreadReference& operator=( KTThreadReference&& );

        public:
            //**************************
            // for use within the thread
            //**************************

            void Break( bool doBreakpoint, const XArgs&... dataHandle );

            void SetReturnValue( XArgs... dataHandle );

        public:
            //******************************
            // for use outside of the thread
            //******************************

            KTDataHandle GetReturnValue();
            boost::unique_future< KTDataHandle >& GetDataPtrRetFuture();
            const boost::unique_future< KTDataHandle >& GetDataPtrRetFuture() const;

            void RefreshDataPtrRet();

        private:
            KTDataPtrReturn fDataPtrRet;
            boost::unique_future< KTDataHandle > fDataPtrRetFuture;
    };

    inline void KTThreadReference::SetReturnException( boost::exception_ptr excPtr )
    {
        fDataPtrRet.set_exception( excPtr );
        return;
    }

    inline void KTThreadReference::SetReturnValue( KTDataHandle dataHandle )
    {
        fDataPtrRet.set_value( dataHandle );
        return;
    }

    inline KTDataHandle KTThreadReference::GetReturnValue()
    {
        return fDataPtrRetFuture.get();
    }

    inline boost::unique_future< KTDataHandle >& KTThreadReference::GetDataPtrRetFuture()
    {
        return fDataPtrRetFuture;
    }

    inline const boost::unique_future< KTDataHandle >& KTThreadReference::GetDataPtrRetFuture() const
    {
        return fDataPtrRetFuture;
    }

    inline void KTThreadReference::RefreshDataPtrRet()
    {
        fDataPtrRet = KTDataPtrReturn();
        fDataPtrRetFuture = fDataPtrRet.get_future();
        return;
    }

    inline void KTThreadReference::SetInitiateBreakFunc( const std::function< void() >& initBreakFunc )
    {
        fInitiateBreakFunc = initBreakFunc;
        return;
    }

    inline void KTThreadReference::SetWaitForContinueFunc( const std::function< void( boost_unique_lock& ) >& waitForContFunc )
    {
        fWaitForContinueFunc = waitForContFunc;
        return;
    }

    inline boost::mutex& KTThreadReference::Mutex()
    {
        return fMutex;
    }

    inline const boost::mutex& KTThreadReference::Mutex() const
    {
        return fMutex;
    }


#ifndef THROW_THREADREF_EXCEPTION
#define THROW_THREADREF_EXCEPTION( ref, exc ) \
        try \
        { \
            BOOST_THROW_EXCEPTION( exc ); \
        } \
        catch( ::boost::exception& e ) \
        { \
            ref->SetReturnException( ::boost::current_exception() ); \
        }
#endif // THROW_THREADREF_EXCEPTION

} /* namespace Nymph */

#endif /* KTTHREADREFERENCE_HH_ */
