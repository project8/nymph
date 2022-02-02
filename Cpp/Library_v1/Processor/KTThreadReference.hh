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

            //void SetReturnException( boost::exception_ptr excPtr );

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

            void SetReturnException( boost::exception_ptr excPtr );

            KTDataHandle GetReturnValue();
            boost::unique_future< KTDataHandle >& GetDataPtrRetFuture();
            const boost::unique_future< KTDataHandle >& GetDataPtrRetFuture() const;

            void RefreshDataPtrRet();

        private:
            KTDataPtrReturn fDataPtrRet;
            boost::unique_future< KTDataHandle > fDataPtrRetFuture;
    };

    template< typename... XArgs >
    void KTThreadReference< XArgs... >::SetReturnException( boost::exception_ptr excPtr )
    {
        fDataPtrRet.set_exception( excPtr );
        return;
    }

    template< typename... XArgs >
    void KTThreadReference< XArgs... >::SetReturnValue( XArgs... dataHandle )
    {
        fDataPtrRet.set_value( dataHandle... );
        return;
    }

    template< typename... XArgs >
    KTDataHandle KTThreadReference< XArgs... >::GetReturnValue()
    {
        return fDataPtrRetFuture.get();
    }

    template< typename... XArgs >
    boost::unique_future< KTDataHandle >& KTThreadReference< XArgs... >::GetDataPtrRetFuture()
    {
        return fDataPtrRetFuture;
    }

    template< typename... XArgs >
    const boost::unique_future< KTDataHandle >& KTThreadReference< XArgs... >::GetDataPtrRetFuture() const
    {
        return fDataPtrRetFuture;
    }

    template< typename... XArgs >
    void KTThreadReference< XArgs... >::RefreshDataPtrRet()
    {
        fDataPtrRet = KTDataPtrReturn();
        fDataPtrRetFuture = fDataPtrRet.get_future();
        return;
    }

    inline void KTThreadReferenceBase::SetInitiateBreakFunc( const std::function< void() >& initBreakFunc )
    {
        fInitiateBreakFunc = initBreakFunc;
        return;
    }

    inline void KTThreadReferenceBase::SetWaitForContinueFunc( const std::function< void( boost_unique_lock& ) >& waitForContFunc )
    {
        fWaitForContinueFunc = waitForContFunc;
        return;
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
