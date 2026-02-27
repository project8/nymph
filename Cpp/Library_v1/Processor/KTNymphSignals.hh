/*
 * KTNymphSignals.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTNYMPHSIGNALS_HH_
#define NYMPH_KTNYMPHSIGNALS_HH_

#include "KTSignal.hh"

#include "KTCoreData.hh"

namespace Nymph
{
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

} /* namespace Nymph */

#endif /* NYMPH_KTNYMPHSIGNALS_HH_ */
