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

    KTThreadReference::KTThreadReference() :
            fName(),
            fBreakFlag( false ),
            fCanceled( false ),
            fDataPtrRet(),
            fDataPtrRetFuture( fDataPtrRet.get_future() ),
            fInitiateBreakFunc( [](){return;} ),
            fContinueSignal()
    {
        if( ! fDataPtrRetFuture.valid() )
        {
            KTERROR( trlog, "Invalid data-pointer-return future created" );
            BOOST_THROW_EXCEPTION( KTException() << "Invalid data-pointer-return future created" << eom );
        }
    }

    KTThreadReference::KTThreadReference( KTThreadReference&& orig ) :
            fName( std::move( orig.fName ) ),
            fBreakFlag( orig.fBreakFlag ),
            fCanceled( orig.fCanceled ),
            fDataPtrRet( std::move( orig.fDataPtrRet ) ),
            fDataPtrRetFuture( std::move( orig.fDataPtrRetFuture ) ),
            fInitiateBreakFunc( std::move( orig.fInitiateBreakFunc ) ),
            fContinueSignal( std::move( orig.fContinueSignal ) )
    {
        orig.fBreakFlag = false;
        orig.fCanceled = false;
    }

    KTThreadReference& KTThreadReference::operator=( KTThreadReference&& orig )
    {
        fName = std::move( orig.fName );
        fBreakFlag = orig.fBreakFlag;
        fCanceled = orig.fCanceled;
        fDataPtrRet = std::move( orig.fDataPtrRet );
        fDataPtrRetFuture = std::move( orig.fDataPtrRetFuture );
        fInitiateBreakFunc = std::move( orig.fInitiateBreakFunc );
        fContinueSignal = std::move( orig.fContinueSignal );

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
            fContinueSignal.wait();
        }
        return;
    }


} /* namespace Nymph */
