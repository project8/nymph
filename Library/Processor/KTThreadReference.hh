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

    class KTThreadReference
    {
        private:
            typedef boost::unique_lock< boost::mutex > boost_unique_lock;

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

            void Break( const KTDataHandle& dataHandle, bool doBreakpoint  );

            void SetReturnException( boost::exception_ptr excPtr );
            void SetReturnValue( KTDataHandle dataHandle );

        public:
            //******************************
            // for use outside of the thread
            //******************************

            KTDataHandle GetReturnValue();
            boost::unique_future< KTDataHandle >& GetDataPtrRetFuture();
            const boost::unique_future< KTDataHandle >& GetDataPtrRetFuture() const;

            void RefreshDataPtrRet();

            void SetInitiateBreakFunc( const std::function< void() >& initBreakFunc );
            void SetWaitForContinueFunc( const std::function< void( boost_unique_lock& ) >& waitForContFunc );

            boost::mutex& Mutex();
            const boost::mutex& Mutex() const;

        public:
            MEMBERVARIABLE_REF( std::string, Name );
            MEMBERVARIABLE( bool, BreakFlag );
            MEMBERVARIABLE( bool, Canceled );

        private:
            KTDataPtrReturn fDataPtrRet;
            boost::unique_future< KTDataHandle > fDataPtrRetFuture;
            std::function< void() > fInitiateBreakFunc;
            std::function< void( boost_unique_lock& ) > fWaitForContinueFunc;
            boost::mutex fMutex;
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
