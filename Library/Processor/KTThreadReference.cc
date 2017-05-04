/*
 * KTThreadReference.cc
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#include "KTThreadReference.hh"

//#include "KTProcessorToolbox.hh"

namespace Nymph
{

    KTThreadReference::KTThreadReference() :
            fDataPtrRet(),
            //fProcTB( nullptr ),
            fInitiateBreakFunc(),
            fRefreshFutureFunc(),
            fThread( nullptr ),
            fThreadIndicator( nullptr )
    {}

    KTThreadReference::KTThreadReference( KTThreadReference&& orig ) :
            fDataPtrRet( std::move( orig.fDataPtrRet ) ),
            //fProcTB( orig.fProcTB ),
            fInitiateBreakFunc( std::move( orig.fInitiateBreakFunc ) ),
            fRefreshFutureFunc( std::move( orig.fRefreshFutureFunc ) ),
            fThread( orig.fThread ),
            fThreadIndicator( orig.fThreadIndicator )
    {
        //orig.fProcTB = nullptr;
        orig.fThread = nullptr;
        orig.fThreadIndicator = nullptr;
    }

    KTThreadReference& KTThreadReference::operator=( KTThreadReference&& orig )
    {
        fDataPtrRet = std::move( orig.fDataPtrRet );
        //fProcTB = orig.fProcTB;
        fInitiateBreakFunc = std::move( orig.fInitiateBreakFunc );
        fRefreshFutureFunc = std::move( orig.fRefreshFutureFunc );
        fThread = orig.fThread;
        fThreadIndicator = orig.fThreadIndicator;

        //orig.fProcTB = nullptr;
        orig.fThread = nullptr;
        orig.fThreadIndicator = nullptr;

        return *this;
    }


    void KTThreadReference::Break( const KTDataPtr& dataPtr  )
    {
        bool breakInititatedHere = false;
        if( /* breakpoint is set here */ false )
        {
            breakInititatedHere = true;
            fInitiateBreakFunc();
            //fProcTB->InitiateBreak();
        }
        if( fThreadIndicator->fBreakFlag || breakInititatedHere )
        {
            fDataPtrRet.set_value( dataPtr );
            fThreadIndicator->fContinueSignal.wait();
            fDataPtrRet = KTDataPtrReturn();
            //fProcTB->TakeFuture( fDataPtrRet.get_future() );
            fRefreshFutureFunc( fDataPtrRet.get_future() );
        }
        return;
    }

} /* namespace Nymph */
