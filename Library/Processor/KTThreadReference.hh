/*
 * KTThreadReference.hh
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#ifndef KTTHREADREFERENCE_HH_
#define KTTHREADREFERENCE_HH_

#include "KTData.hh"

#include <boost/thread/future.hpp>

#include <memory>

namespace Nymph
{
    typedef boost::promise< KTDataPtr > KTDataPtrReturn;

    class KTThreadReference
    {
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

            void Break( const KTDataPtr& dataPtr, bool doBreakpoint  );

            void SetReturnException( boost::exception_ptr excPtr );
            void SetReturnValue( KTDataPtr dataPtr );

        public:
            //******************************
            // for use outside of the thread
            //******************************

            KTDataPtr GetReturnValue();
            boost::unique_future< KTDataPtr >& GetDataPtrRetFuture();
            const boost::unique_future< KTDataPtr >& GetDataPtrRetFuture() const;

            void RefreshDataPtrRet();

            void SetContinueSignal( const boost::shared_future< void >& contSig );
            void SetInitiateBreakFunc( const std::function< void() >& initBreakFunc );

        public:
            MEMBERVARIABLEREF( std::string, Name );
            MEMBERVARIABLE( bool, BreakFlag );
            MEMBERVARIABLE( bool, Canceled );

        private:
            KTDataPtrReturn fDataPtrRet;
            boost::unique_future< KTDataPtr > fDataPtrRetFuture;
            std::function< void() > fInitiateBreakFunc;
            boost::shared_future< void > fContinueSignal;
    };

    inline void KTThreadReference::SetReturnException( boost::exception_ptr excPtr )
    {
        fDataPtrRet.set_exception( excPtr );
        return;
    }

    inline void KTThreadReference::SetReturnValue( KTDataPtr dataPtr )
    {
        fDataPtrRet.set_value( dataPtr );
        return;
    }

    inline KTDataPtr KTThreadReference::GetReturnValue()
    {
        return fDataPtrRetFuture.get();
    }

    inline boost::unique_future< KTDataPtr >& KTThreadReference::GetDataPtrRetFuture()
    {
        return fDataPtrRetFuture;
    }

    inline const boost::unique_future< KTDataPtr >& KTThreadReference::GetDataPtrRetFuture() const
    {
        return fDataPtrRetFuture;
    }

    inline void KTThreadReference::RefreshDataPtrRet()
    {
        fDataPtrRet = KTDataPtrReturn();
        fDataPtrRetFuture = fDataPtrRet.get_future();
        return;
    }

    inline void KTThreadReference::SetContinueSignal( const boost::shared_future< void >& contSig )
    {
        fContinueSignal = contSig;
        return;
    }

    inline void KTThreadReference::SetInitiateBreakFunc( const std::function< void() >& initBreakFunc )
    {
        fInitiateBreakFunc = initBreakFunc;
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
