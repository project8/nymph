/*
 * KTThreadReference.cc
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#include "KTThreadReference.hh"

#include "KTLogger.hh"

KTLOGGER( trlog, "KTThreadReference" );

namespace Nymph
{
    KTThreadIndicator::KTThreadIndicator() :
            fBreakFlag( false ),
            fContinueSignal(),
            fCanceled( false )
    {}

    KTThreadReference::KTThreadReference() :
            fDataPtrRet(),
            fInitiateBreakFunc( [](){return;} ),
            fRefreshFutureFunc( [](const std::string&, boost::shared_future< KTDataPtr >&&){return;} ),
            fThreadIndicator( new KTThreadIndicator() ),
            fPrimaryProcName()
    {}

    KTThreadReference::KTThreadReference( KTThreadReference&& orig ) :
            fDataPtrRet( std::move( orig.fDataPtrRet ) ),
            fInitiateBreakFunc( std::move( orig.fInitiateBreakFunc ) ),
            fRefreshFutureFunc( std::move( orig.fRefreshFutureFunc ) ),
            fThreadIndicator( orig.fThreadIndicator ),
            fPrimaryProcName( std::move( orig.fPrimaryProcName ) )
    {
        orig.fThreadIndicator = nullptr;
    }

    KTThreadReference& KTThreadReference::operator=( KTThreadReference&& orig )
    {
        fDataPtrRet = std::move( orig.fDataPtrRet );
        fInitiateBreakFunc = std::move( orig.fInitiateBreakFunc );
        fRefreshFutureFunc = std::move( orig.fRefreshFutureFunc );
        fThreadIndicator = orig.fThreadIndicator;
        fPrimaryProcName = std::move( orig.fPrimaryProcName );

        orig.fThreadIndicator = nullptr;

        return *this;
    }

    void KTThreadReference::Break( const KTDataPtr& dataPtr, bool doBreakpoint )
    {
        if( doBreakpoint )
        {
            KTDEBUG( trlog, "Initiating break" );
            fInitiateBreakFunc();
        }
        if( fThreadIndicator->fBreakFlag || doBreakpoint )
        {
            KTDEBUG( trlog, "Reacting to break" );
            // set the return for this thread
            fDataPtrRet.set_value( dataPtr );
            // wait for continue signal
            fThreadIndicator->fContinueSignal.wait();
            // reset the promise
            fDataPtrRet = KTDataPtrReturn();
            // pass the future back to the processor toolbox (if it's in use)
            fRefreshFutureFunc( fPrimaryProcName, std::move( fDataPtrRet.get_future().share() ) );
        }
        return;
    }

} /* namespace Nymph */
