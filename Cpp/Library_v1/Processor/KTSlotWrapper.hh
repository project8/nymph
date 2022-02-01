/*
 * KTSlotWrapper.hh
 *
 *  Created on: Feb 10, 2012
 *      Author: nsoblath
 */

#ifndef KTSLOTWRAPPER_HH_
#define KTSLOTWRAPPER_HH_

#include "KTConnection.hh"
#include "KTException.hh"
#include "KTSignalWrapper.hh"
#include "KTThreadReference.hh"

#include <boost/signals2.hpp>

namespace Nymph
{
    struct KTConnectionException : public KTException
    {};

    class KTSlotWrapper : public boost::noncopyable
    {
        private:
            class KTInternalSlotWrapper
            {
                public:
                    KTInternalSlotWrapper() {}
                    virtual ~KTInternalSlotWrapper() {}

                    virtual KTConnection Connect(KTSignalWrapper* signalWrap, int groupNum=-1) = 0;
            };

            template< typename XSignature, typename XTypeContainer >
            class KTSpecifiedInternalSlotWrapper : public KTInternalSlotWrapper, public boost::noncopyable
            {
                public:
                    KTSpecifiedInternalSlotWrapper(XSignature funcPtr, XTypeContainer* typeCont=NULL) :
                            fSlot(funcPtr)
                    {
                        (void)typeCont; // to suppress warnings
                    }
                    virtual ~KTSpecifiedInternalSlotWrapper()
                    {
                    }

                    virtual KTConnection Connect(KTSignalWrapper* signalWrap, int groupNum=-1)
                    {
                        typedef KTSignalWrapper::KTInternalSignalWrapper SignalWrapperBase;
                        typedef KTSignalWrapper::KTSpecifiedInternalSignalWrapper< typename XTypeContainer::signal > SignalWrapper;

                        SignalWrapperBase* internalSignalWrap = signalWrap->GetInternal();
                        SignalWrapper* derivedSignalWrapper = dynamic_cast< SignalWrapper* >(internalSignalWrap);
                        if (derivedSignalWrapper == NULL)
                        {
                            BOOST_THROW_EXCEPTION( KTConnectionException() << "Cannot make connection: unable to cast from KTInternalSignalWrapper* to this slot's derived type." << eom );
                        }
                        if (groupNum >= 0)
                        {
                            return derivedSignalWrapper->GetSignal()->connect(groupNum, fSlot);
                        }
                        return derivedSignalWrapper->GetSignal()->connect(fSlot);
                    }

                private:
                    XSignature fSlot;
            };

        public:
            template< typename XSignature, typename XTypeContainer >
            KTSlotWrapper(XSignature signalPtr, XTypeContainer* typeCont);
            ~KTSlotWrapper();

        private:
            KTSlotWrapper();

            KTInternalSlotWrapper* fSlotWrapper;

        public:
            void SetConnection(KTConnection conn);
            void SetConnection(KTSignalWrapper* signalWrap, int groupNum=-1); // can throw KTConnectionException
            void Disconnect();

        private:
            KTConnection fConnection;

        public:
            std::shared_ptr< KTThreadReference > GetThreadRef() const;
            void SetThreadRef(std::shared_ptr< KTThreadReference > ref);

            bool GetDoBreakpoint() const;
            void SetDoBreakpoint(bool flag);

        private:
            std::shared_ptr< KTThreadReference > fThreadRef;
            bool fDoBreakpoint;

    };

    template< typename XSignature, typename XTypeContainer >
    KTSlotWrapper::KTSlotWrapper(XSignature signalPtr, XTypeContainer* typeCont) :
            fSlotWrapper(new KTSpecifiedInternalSlotWrapper< XSignature, XTypeContainer >(signalPtr, typeCont)),
            fConnection(),
            fThreadRef( std::make_shared< KTThreadReference >() ),
            fDoBreakpoint(false)
    {
    }

    inline void KTSlotWrapper::SetConnection(KTConnection conn)
    {
        fConnection = conn;
        return;
    }

    inline void KTSlotWrapper::SetConnection(KTSignalWrapper* signalWrap, int groupNum)
    {
        fConnection = this->fSlotWrapper->Connect(signalWrap, groupNum);
        return;
    }

    inline void KTSlotWrapper::Disconnect()
    {
        fConnection.disconnect();
        return;
    }

    inline std::shared_ptr< KTThreadReference > KTSlotWrapper::GetThreadRef() const
    {
        return fThreadRef;
    }

    inline void KTSlotWrapper::SetThreadRef(std::shared_ptr< KTThreadReference > ref)
    {
        fThreadRef = ref;
        return;
    }

    inline bool KTSlotWrapper::GetDoBreakpoint() const
    {
        return fDoBreakpoint;
    }

    inline void KTSlotWrapper::SetDoBreakpoint(bool flag)
    {
        fDoBreakpoint = flag;
        return;
    }


} /* namespace Nymph */
#endif /* KTSLOTWRAPPER_HH_ */
