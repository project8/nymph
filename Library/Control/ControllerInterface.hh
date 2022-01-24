/*
 * ControllerInterface.hh
 *
 *  Created on: Jan 24, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLLERINTERFACE_HH_
#define NYMPH_CONTROLLERINTERFACE_HH_

#include <exception>
#include <string>

namespace Nymph
{

    class ControllerInterface
    {
        public:
            ControllerInterface();
            virtual ~ControllerInterface();

            virtual bool WaitToContinue() = 0;

            /// Returns when processing is completed or a breakpoint is reached
            /// If the return is true, processing can continue after the break
            /// If the return is false, processing has ended (either normally or due to an error)
            virtual bool WaitForBreakOrCanceled() = 0;

            /// Use this to have a thread wait for the end of a run
            virtual void WaitForEndOfRun() = 0;

            /// Instruct the Controller to continue after a breakpoint
            virtual void Continue() = 0;

            /// Cancel all threads and end the run
            virtual void Cancel( int code = 0 ) = 0;

            /// Reports whether controls is canceled
            virtual bool IsCanceled() const = 0;

            /// Initiate a break
            virtual void Break() = 0;

            /// Reports whether control is at a breakpoint
            virtual bool IsAtBreak() const = 0;

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr ) = 0;
    };

} /* namespace Nymph */

#endif /* NYMPH_CONTROLLERINTERFACE_HH_ */
