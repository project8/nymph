/*
 * KTSignal.hh
 *
 *  Created on: Jan 15, 2013
 *      Author: nsoblath
 */

#ifndef KTSIGNAL_HH_
#define KTSIGNAL_HH_

#include "KTProcessor.hh"

#include "KTData.hh"

#include <boost/signals2.hpp>

#include <string>

namespace Nymph
{
    /*!
     @class KTSignalOneArg
     @author N. S. Oblath

     @brief Creates a signal that takes a single argument.

     @details
     The signal is emitted by calling operator().
     If a KTDataSlot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type KTDataSignal< ArgumentType >.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     To use the signal, call it as: fSignalObject(arg);
    */
    template< class XSignalArgument >
    class KTSignalOneArg
    {
        public:
            typedef void (signature)(XSignalArgument);
            typedef boost::signals2::signal< signature > boost_signal;
            typedef typename boost::signals2::signal< signature >::slot_type slot_type;

        public:
            KTSignalOneArg();
            KTSignalOneArg(const std::string& name, KTProcessor* proc);
            virtual ~KTSignalOneArg();

        protected:
            KTSignalOneArg(const KTSignalOneArg&);

        public:
            void operator()(XSignalArgument arg);

            boost_signal* Signal();

        protected:
            boost_signal fSignal;

    };


    template<>
    class KTSignalOneArg< void >
    {
        public:
            typedef void (signature)(void);
            typedef boost::signals2::signal< signature > boost_signal;
            // the following line is, technically, not valid C++03 code because the 'typename' keyword
            // is appearing outside of a template (since the template here is fully specified, this counts).
            // It should compiler with most new-ish compilers, even when compiling under C++03 mode.
            // It has been tested successfully with GCC 4.6 and Clang 3.1
            typedef typename boost::signals2::signal< signature >::slot_type slot_type;

        public:
            KTSignalOneArg();
            KTSignalOneArg(const std::string& name, KTProcessor* proc);
            virtual ~KTSignalOneArg();

        protected:
            KTSignalOneArg(const KTSignalOneArg&);

        public:
            void operator()();

            boost_signal* Signal();

        protected:
            boost_signal fSignal;

    };

    // convenience typedef for KTSignalDone
    typedef KTSignalOneArg<void> KTSignalDone;



    /*!
     @class KTSignalData
     @author N. S. Oblath

     @brief Creates a signal that takes a KTDataPtr object as its argument.

     @details
     The purpose of the signal is for passing KTData pointers between Processors.
     The signal is emitted by calling operator().
     If a KTDataSlot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type KTSignalData.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     That's it!
    */

    class KTSignalData : public KTSignalOneArg< KTDataPtr >
    {
        public:
            typedef void (signature)(KTDataPtr);
            typedef boost::signals2::signal< signature > boost_signal;
            typedef boost::signals2::signal< signature >::slot_type slot_type;

            typedef void (ref_signature)(KTDataPtr&);
            typedef boost::signals2::signal< ref_signature > ref_boost_signal;
            typedef boost::signals2::signal< ref_signature >::slot_type ref_slot_type;

        public:
            KTSignalData();
            KTSignalData(const std::string& name, KTProcessor* proc);
            virtual ~KTSignalData();

        protected:
            KTSignalData(const KTSignalData&);

        public:
            void operator()(KTDataPtr arg);

            ref_boost_signal* RefSignal();

        protected:
            ref_boost_signal fRefSignal;
    };



    template< class XSignalArgument >
    KTSignalOneArg< XSignalArgument >::KTSignalOneArg(const std::string& name, KTProcessor* proc) :
            fSignal()
    {
        proc->RegisterSignal(name, &fSignal);
    }

    template< class XSignalArgument >
    KTSignalOneArg< XSignalArgument >::KTSignalOneArg() :
            fSignal()
    {}

    template< class XSignalArgument >
    KTSignalOneArg< XSignalArgument >::KTSignalOneArg(const KTSignalOneArg&) :
            fSignal()
    {}

    template< class XSignalArgument >
    KTSignalOneArg< XSignalArgument >::~KTSignalOneArg()
    {
    }

    template< class XSignalArgument >
    inline void KTSignalOneArg< XSignalArgument >::operator()(XSignalArgument arg)
    {
        fSignal(arg);
    }

    template< class XSignalArgument >
    inline typename KTSignalOneArg< XSignalArgument >::boost_signal* KTSignalOneArg< XSignalArgument >::Signal()
    {
        return &fSignal;
    }


    inline void KTSignalOneArg< void >::operator()()
    {
        fSignal();
    }

    inline typename KTSignalOneArg< void >::boost_signal* KTSignalOneArg< void >::Signal()
    {
        return &fSignal;
    }


    inline void KTSignalData::operator()(KTDataPtr arg)
    {
        fSignal(arg);
        fRefSignal(arg);
    }

    inline typename KTSignalData::ref_boost_signal* KTSignalData::RefSignal()
    {
        return &fRefSignal;
    }

} /* namespace Nymph */
#endif /* KTSIGNAL_HH_ */
