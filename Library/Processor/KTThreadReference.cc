/*
 * KTThreadReference.cc
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#include "KTThreadReference.hh"

#include "KTException.hh"
#include "KTLogger.hh"

KTLOGGER( trlog, "KTThreadReference" );

namespace Nymph
{
    //KTThreadIndicator::KTThreadIndicator() :
    //        fBreakFlag( false ),
    //        fContinueSignal(),
    //        fCanceled( false )
    //{}

    KTThreadReference::KTThreadReference() :
            fName(),
            fDataPtrRet(),
            fDataPtrRetFuture( fDataPtrRet.get_future() ),
            fInitiateBreakFunc( [](){return;} ),
            fRefreshFutureFunc( [](const std::string&, boost::shared_future< KTDataPtr >&&){return;} ),
            //fThreadIndicator( new KTThreadIndicator() ),
            fBreakFlag( false ),
            fContinueSignal(),
            fCanceled( false ),
            fPrimaryProcName()
    {
        if( ! fDataPtrRetFuture.valid() )
        {
            KTERROR( trlog, "Invalid data-pointer-return future created" );
            BOOST_THROW_EXCEPTION( KTException() << "Invalid data-pointer-return future created" << eom );
        }
    }

    KTThreadReference::KTThreadReference( KTThreadReference&& orig ) :
            fName( std::move( orig.fName ) ),
            fDataPtrRet( std::move( orig.fDataPtrRet ) ),
            fDataPtrRetFuture( std::move( orig.fDataPtrRetFuture ) ),
            fInitiateBreakFunc( std::move( orig.fInitiateBreakFunc ) ),
            fRefreshFutureFunc( std::move( orig.fRefreshFutureFunc ) ),
            //fThreadIndicator( orig.fThreadIndicator ),
            fBreakFlag( orig.fBreakFlag ),
            fContinueSignal( std::move( orig.fContinueSignal ) ),
            fCanceled( orig.fCanceled ),
            fPrimaryProcName( std::move( orig.fPrimaryProcName ) )
    {
        //orig.fThreadIndicator = nullptr;
        orig.fBreakFlag = false;
        orig.fCanceled = false;
    }

    KTThreadReference& KTThreadReference::operator=( KTThreadReference&& orig )
    {
        fName = std::move( orig.fName );
        fDataPtrRet = std::move( orig.fDataPtrRet );
        fDataPtrRetFuture = std::move( orig.fDataPtrRetFuture );
        fInitiateBreakFunc = std::move( orig.fInitiateBreakFunc );
        fRefreshFutureFunc = std::move( orig.fRefreshFutureFunc );
        //fThreadIndicator = orig.fThreadIndicator;
        fBreakFlag = orig.fBreakFlag;
        fContinueSignal = std::move( orig.fContinueSignal );
        fCanceled = orig.fCanceled;
        fPrimaryProcName = std::move( orig.fPrimaryProcName );

        //orig.fThreadIndicator = nullptr;
        orig.fBreakFlag = false;
        orig.fCanceled = false;

        return *this;
    }

    void KTThreadReference::Break( const KTDataPtr& dataPtr, bool doBreakpoint )
    {
        if( doBreakpoint )
        {
            KTDEBUG( trlog, "Initiating break (" << fName << ")" );
            fInitiateBreakFunc();
        }
        if( fBreakFlag || doBreakpoint )
        {
            KTDEBUG( trlog, "Reacting to break (" << fName << ")" );
            // set the return for this thread
            KTWARN( trlog, "Setting value of data-ptr-ret promise (" << fName << ")" );
            fDataPtrRet.set_value( dataPtr );
            // wait for continue signal
            //fThreadIndicator->fContinueSignal.wait();
            fContinueSignal.wait();
            // reset the promise
            //fDataPtrRet = KTDataPtrReturn();
            // pass the future back to the processor toolbox (if it's in use)
            //fRefreshFutureFunc( fPrimaryProcName, std::move( fDataPtrRet.get_future().share() ) );
        }
        return;
    }

    void KTThreadReference::RefreshDataPtrRet()
    {
        fDataPtrRet = KTDataPtrReturn();
        fDataPtrRetFuture = fDataPtrRet.get_future();
        return;
    }

} /* namespace Nymph */
