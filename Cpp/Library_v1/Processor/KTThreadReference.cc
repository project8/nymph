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
            fWaitForContinueFunc( []( boost_unique_lock& ){return;} ),
            fMutex()
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
            fWaitForContinueFunc( std::move( orig.fWaitForContinueFunc ) ),
            fMutex()
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
        fWaitForContinueFunc = std::move( orig.fWaitForContinueFunc );

        orig.fBreakFlag = false;
        orig.fCanceled = false;

        return *this;
    }

    void KTThreadReference::Break( const KTDataHandle& dataHandle, bool doBreakpoint )
    {
        if( doBreakpoint )
        {
            KTDEBUG( trlog, "Initiating break (" << fName << ")" );
            fInitiateBreakFunc();
        }
        boost_unique_lock lock( fMutex );
        if( fBreakFlag || doBreakpoint )
        {
            KTDEBUG( trlog, "Reacting to break (" << fName << ")" );
            // set the return for this thread
            KTWARN( trlog, "Setting value of data-ptr-ret promise (" << fName << ")" );
            fDataPtrRet.set_value( dataHandle );
            // wait for continue signal
            fWaitForContinueFunc( lock );
        }
        return;
    }


} /* namespace Nymph */
