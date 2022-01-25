/*
 * SignalData.hh
 *
 *  Created on: Jan 25, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_SIGNALDATA_HH_
#define NYMPH_SIGNALDATA_HH_

#include "Signal.hh"

#include "DataFrame.hh"

namespace Nymph
{
    /*!
     @class SignalData
     @author N. S. Oblath

     @brief Creates a signal that takes a DataFrame object as its argument.

     @details
     The purpose of the signal is for passing DataFrame pointers between Processors.

     // TODO: remove this comment; perhaps move it to Slot or SlotData
     If a DataSlot is being used to receive the DataFrame that this Signal is supposed to emit
     then the Slot can be given a pointer to this Signal and the generic Slot function will automatically emit this Signal.

     Usage:
     In your Processor's header add a member variable of type SignalData:

         SignalData fMySignal;

     Initialize the signal with the processor's 'this' pointer and the name of the signal:

        fMySignal( "my-signal", this )

     That's it!
    */
    typedef KTSignal< DataFrame > SignalData;

} /* namespace Nymph */

#endif /* NYMPH_SIGNALDATA_HH_ */
