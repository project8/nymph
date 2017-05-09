/*
 * KTSlotWrapper.hh
 *
 *  Created on: Feb 10, 2012
 *      Author: nsoblath
 */

#ifndef KTSLOTWRAPPER_HH_
#define KTSLOTWRAPPER_HH_

#include "KTConnection.hh"
#include "KTSignalWrapper.hh"
#include "KTThreadReference.hh"

#include <boost/signals2.hpp>

namespace Nymph
{
    class SlotException : public std::logic_error
    {
        public:
            SlotException(std::string const& why);
    };

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
                            throw SignalException("In KTSpecifiedInternalSlotWrapper::Connect:\nUnable to cast from KTInternalSignalWrapper* to derived type.");
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
            void SetConnection(KTSignalWrapper* signalWrap, int groupNum=-1);
            void Disconnect();

        private:
            KTConnection fConnection;

        public:
            KTThreadReference* GetThreadRef() const;
            void SetThreadRef(KTThreadReference* ref);

            bool GetDoBreakpoint() const;
            void SetDoBreakpoint(bool flag);

        private:
            KTThreadReference* fThreadRef;
            bool fDoBreakpoint;

    };

    template< typename XSignature, typename XTypeContainer >
    KTSlotWrapper::KTSlotWrapper(XSignature signalPtr, XTypeContainer* typeCont) :
            fSlotWrapper(new KTSpecifiedInternalSlotWrapper< XSignature, XTypeContainer >(signalPtr, typeCont)),
            fConnection(),
            fThreadRef(),
            fDoBreakpoint(false)
    {}

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

    inline KTThreadReference* KTSlotWrapper::GetThreadRef() const
    {
        return fThreadRef;
    }

    inline void KTSlotWrapper::SetThreadRef(KTThreadReference* ref)
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
