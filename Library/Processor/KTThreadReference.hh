/*
 * KTThreadReference.hh
 *
 *  Created on: May 2, 2017
 *      Author: obla999
 */

#ifndef KTTHREADREFERENCE_HH_
#define KTTHREADREFERENCE_HH_

#include "KTData.hh"

#include <boost/thread/future.hpp>

#include <memory>

namespace Nymph
{
    struct KTThreadIndicator
    {
        bool fBreakFlag; // only use outside of blocks protected by a mutex are reads, so we shouldn't need to make this an atomic
        boost::shared_future< void > fContinueSignal;
        bool fCanceled;
        KTThreadIndicator();
    };

    struct KTThreadReference
    {
        KTDataPtrReturn fDataPtrRet;
        std::function< void() > fInitiateBreakFunc;
        std::function< void(std::future< KTDataPtr >&&) > fRefreshFutureFunc;
        std::shared_ptr< KTThreadIndicator > fThreadIndicator;

        KTThreadReference();
        KTThreadReference( const KTThreadReference& ) = delete;
        KTThreadReference( KTThreadReference&& orig );

        KTThreadReference& operator=( const KTThreadReference& ) = delete;
        KTThreadReference& operator=( KTThreadReference&& );

        void Break( const KTDataPtr& dataPtr, bool doBreakpoint  );
    };

} /* namespace Nymph */

#endif /* KTTHREADREFERENCE_HH_ */
