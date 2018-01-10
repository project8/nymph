/*
 * KTSignal.hh
 *
 *  Created on: Jan 15, 2013
 *      Author: nsoblath
 */

#ifndef KTSIGNAL_HH_
#define KTSIGNAL_HH_

#include "KTProcessor.hh"

#include "KTCoreData.hh"

#include <boost/signals2.hpp>

#include <string>

namespace Nymph
{
    /*!
     @class KTSignal
     @author N. S. Oblath

     @brief Creates a signal that takes 0 or more arguments.

     @details
     The signal is emitted by calling operator().
     If a KTSlot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type KTSignal< ArgumentTypes >.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     To use the signal, call it as: fSignalObject(arg);
    */
    template< class... XSignalArguments >
    class KTSignal
    {
        public:
            typedef void (signature)( XSignalArguments... );
            typedef boost::signals2::signal< signature > boost_signal;
            typedef typename boost::signals2::signal< signature >::slot_type slot_type;

        public:
            KTSignal();
            KTSignal( const std::string& name, KTProcessor* proc );
            virtual ~KTSignal();

        protected:
            KTSignal( const KTSignal& );

        public:
            void operator()( XSignalArguments... args );

            boost_signal* Signal();

            const std::string& GetName() const;

        protected:
            boost_signal fSignal;

            std::string fName;
    };

    /// Convenience typedef for done signals
    typedef KTSignal<> KTSignalDone;

    // Convenience typedef for backwards compatibility
    template< typename XDataType >
    using KTSignalOneArg = KTSignal< XDataType >;

    /*!
     @class KTSignalData
     @author N. S. Oblath

     @brief Creates a signal that takes a KTDataHandle object as its argument.

     @details
     The purpose of the signal is for passing KTCoreData pointers between Processors.
     The signal is emitted by calling operator().
     If a KTDataSlot is being used, and the Slot has been given a pointer to this signal, the Slot will emit the Signal.

     Usage:
     In your Processor's header add a member variable of type KTSignalData.

     Initialize the signal with the processor's 'this' pointer and the name of the signal.

     That's it!
    */
    typedef KTSignal< KTDataHandle > KTSignalData;


    //*******************
    // Implementations
    //*******************

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal( const std::string& name, KTProcessor* proc ) :
            fSignal(),
            fName( name )
    {
        proc->RegisterSignal(name, &fSignal);
    }

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal() :
            fSignal(),
            fName("none")
    {}

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::KTSignal( const KTSignal& signal ) :
            fSignal(),
            fName( signal.fName )
    {}

    template< class... XSignalArguments >
    KTSignal< XSignalArguments... >::~KTSignal()
    {
    }

    template< class... XSignalArguments >
    inline void KTSignal< XSignalArguments... >::operator()( XSignalArguments... args )
    {
        fSignal( args... );
    }

    template< class... XSignalArguments >
    inline typename KTSignal< XSignalArguments... >::boost_signal* KTSignal< XSignalArguments... >::Signal()
    {
        return &fSignal;
    }

    template< class... XSignalArguments >
    inline const std::string& KTSignal< XSignalArguments... >::GetName() const
    {
        return fName;
    }

} /* namespace Nymph */
#endif /* KTSIGNAL_HH_ */
