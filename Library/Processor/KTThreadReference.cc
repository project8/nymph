/*
 * KTThreadReference.cc
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#include "KTThreadReference.hh"

namespace Nymph
{

    void KTThreadReference::Break( const KTDataPtr& dataPtr  )
    {
        bool breakInititatedHere = false;
        if( /* breakpoint is set here */ false )
        {
            breakInititatedHere = true;
            fProcTB->InitiateBreak();
        }
        if( fThreadIndicator->fBreakFlag || breakInititatedHere )
        {
            fDataPtrRet.set_value( dataPtr );
            fThreadIndicator->fContinueSignal.wait();
            fDataPtrRet = KTDataPtrReturn();
            fProcTB->TakeFuture( fDataPtrRet.get_future() );
        }
        return;
    }

} /* namespace Nymph */
