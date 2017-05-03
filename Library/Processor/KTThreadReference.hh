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

    struct KTThreadReference
    {
        KTDataPtrReturn fDataPtrRet;
        KTProcessorToolbox* fProcTB;
        bool fBreakFlag; // only use outside of blocks protected by fBreakContMutex are reads, so we shouldn't need to make this an atomic
        std::shared_future< void > fContinueSignal;
        std::thread* fThread;

        void Break( const KTDataPtr& dataPtr  );
    };

} /* namespace Nymph */

#endif /* KTTHREADREFERENCE_HH_ */
