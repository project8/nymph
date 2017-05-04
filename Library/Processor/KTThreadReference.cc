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
    KTThreadIndicator::KTThreadIndicator() :
            fBreakFlag( false ),
            fContinueSignal(),
            fCanceled( false )
    {}

    KTThreadReference::KTThreadReference() :
            fDataPtrRet(),
            fInitiateBreakFunc(),
            fRefreshFutureFunc(),
            fThreadIndicator( nullptr )
    {}

    KTThreadReference::KTThreadReference( KTThreadReference&& orig ) :
            fDataPtrRet( std::move( orig.fDataPtrRet ) ),
            fInitiateBreakFunc( std::move( orig.fInitiateBreakFunc ) ),
            fRefreshFutureFunc( std::move( orig.fRefreshFutureFunc ) ),
            fThreadIndicator( orig.fThreadIndicator )
    {
        orig.fThreadIndicator = nullptr;
    }

    KTThreadReference& KTThreadReference::operator=( KTThreadReference&& orig )
    {
        fDataPtrRet = std::move( orig.fDataPtrRet );
        fInitiateBreakFunc = std::move( orig.fInitiateBreakFunc );
        fRefreshFutureFunc = std::move( orig.fRefreshFutureFunc );
        fThreadIndicator = orig.fThreadIndicator;

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
        }
        if( fThreadIndicator->fBreakFlag || breakInititatedHere )
        {
            fDataPtrRet.set_value( dataPtr );
            fThreadIndicator->fContinueSignal.wait();
            fDataPtrRet = KTDataPtrReturn();
            fRefreshFutureFunc( fDataPtrRet.get_future() );
        }
        return;
    }

} /* namespace Nymph */
