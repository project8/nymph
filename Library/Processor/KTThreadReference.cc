/*
 * KTThreadReference.cc
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#include "KTThreadReference.hh"

namespace Nymph
{

    KTThreadReference::KTThreadReference() :
            fDataPtrRet(),
            fProcTB( nullptr ),
            fBreakFlag( false ),
            fContinueSignal(),
            fThread( nullptr )
    {
    }

    KTThreadReference::~KTThreadReference()
    {
    }

    void KTThreadReference::Break( const KTDataPtr& dataPtr  )
    {
        bool breakInititatedHere = false;
        if( /* breakpoint is set here */ false )
        {
            breakInititatedHere = true;
            fProcTB->InitiateBreak();
        }
        if( fBreakFlag || breakInititatedHere )
        {
            fDataPtrRet.set_value( dataPtr );
            fContinueSignal.wait();
            fDataPtrRet = KTDataPtrReturn();
            fProcTB->TakeFuture( fDataPtrRet.get_future() );
        }
        return;
    }

} /* namespace Nymph */
