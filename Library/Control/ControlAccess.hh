/*
 * ControlAccess.hh
 *
 *  Created on: Sept 14, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLACCESS_HH_
#define NYMPH_CONTROLACCESS_HH_

#include "Controller.hh"
#include "Exception.hh"
#include "MemberVariable.hh"
#include "ReturnBuffer.hh"

#include "cancelable.hh" // remove when ControlAccess is removed
#include "singleton.hh"

#include <condition_variable>
#include <memory>
#include <mutex>

namespace Nymph
{

    class ControlAccess : public scarab::singleton< ControlAccess >, public ControllerInterface
    {
        protected:
            allow_singleton_access( ControlAccess );

            ControlAccess();
            virtual ~ControlAccess();

        public:
            virtual bool WaitToContinue();

            /// Returns when processing is completed or a breakpoint is reached
            /// If the return is true, processing can continue after the break
            /// If the return is false, processing has ended (either normally or due to an error)
            virtual bool WaitForBreakOrCanceled();

            /// Use this to have a thread wait for the end of a run
            virtual void WaitForEndOfRun();

            /// Instruct the Controller to continue after a breakpoint
            virtual void Continue();

            /// Cancel all threads and end the run
            virtual void Cancel( int code = 0 );

            /// Reports whether controls is canceled
            virtual bool IsCanceled() const;

            /// Initiate a break
            virtual void Break();

            /// Reports whether control is at a breakpoint
            virtual bool IsAtBreak() const;

            /// Notify the control that a chain is quitting
            virtual void ChainQuitting( std::exception_ptr ePtr );

            MEMVAR_PTR( ControllerInterface, Control );

    };



    class ReturnAccess : public scarab::cancelable
    {
        public:
            ReturnAccess();

            virtual ~ReturnAccess();

            template< typename... Args >
            void SetReturn( Args&... arg );

            template< typename... Args >
            std::tuple< Args&... >& GetReturn();

        protected:
            std::shared_ptr< ReturnBufferBase > fReturn;

        public:
            //MEM_VAR_SHARED_PTR( SharedControl, Control );
    };

    typedef std::shared_ptr< ControlAccess > ControlAccessPtr;


    template< typename... Args >
    void ReturnAccess::SetReturn( Args&... args )
    {
        fReturn = std::make_shared< ReturnBuffer< Args... > >( args... );
        return;
    }

    template< typename... Args >
    std::tuple< Args&... >& ReturnAccess::GetReturn()
    {
        std::shared_ptr< ReturnBuffer< Args... > > buffer( std::dynamic_pointer_cast< ReturnBuffer< Args... > >(fReturn) );
        if( buffer == nullptr ) throw std::exception();
        return buffer->GetReturn();
    }

} /* namespace Nymph */

#endif /* NYMPH_CONTROLACCESS_HH_ */
