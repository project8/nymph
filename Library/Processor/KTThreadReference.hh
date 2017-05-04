/*
 * KTThreadReference.hh
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#ifndef KTTHREADREFERENCE_HH_
#define KTTHREADREFERENCE_HH_

#include "KTData.hh"

#include <thread>

namespace Nymph
{
    class KTProcessorToolbox;

    struct KTThreadIndicator
    {
        bool fBreakFlag; // only use outside of blocks protected by a mutex are reads, so we shouldn't need to make this an atomic
        std::shared_future< void > fContinueSignal;
    };

    struct KTThreadReference
    {
        KTDataPtrReturn fDataPtrRet;
        KTProcessorToolbox* fProcTB;
        std::thread* fThread;
        KTThreadIndicator* fThreadIndicator;

        KTThreadReference();
        KTThreadReference( const KTThreadReference& ) = delete;
        KTThreadReference( KTThreadReference&& orig );

        KTThreadReference& operator=( const KTThreadReference& ) = delete;
        KTThreadReference& operator=( KTThreadReference&& );

        void Break( const KTDataPtr& dataPtr  );
    };

} /* namespace Nymph */

#endif /* KTTHREADREFERENCE_HH_ */
