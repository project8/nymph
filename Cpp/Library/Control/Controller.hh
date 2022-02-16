/*
 * Controller.hh
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLLER_HH_
#define NYMPH_CONTROLLER_HH_

#include "MemberVariable.hh"
#include "ReturnBuffer.hh"

#include "cancelable.hh"
#include "param.hh"

#include <condition_variable>
#include <mutex>

#include <iostream>

namespace Nymph
{

    /*!
     @class Controller
     @author N. S. Oblath

     @brief Base class for Controller classes, with basic break/continue/cancel functionality

     @details
     A Controller can:

     * Cancel operations: typically used by either a Processor chain's thread (usually via ControlAccess) or the client of the Controller, this will result in the full cancelation, from the top down, of all Nymph-based operations
     * Initiate a break: used mostly by a Processor chain's thread where a breakpoint is set, a breakpoint can be initiated with control returned to the client; parameters (e.g. data) can be returned at the break to provide access to the client
     * Waiting on a break/cancellation/continuation: the Controller can be used to cause a thread to wait on any of those conditions; cancellation will interupt all wait conditions
     * Be notified of a quitting Processor chain: when a Processor chain quits, it notifies the Controller (via ControlAccess) that it is quitting; if there's an exception, that should be provided to properly forward error conditions
    */
    class Controller : public scarab::cancelable
    {
        public:
            Controller();
            virtual ~Controller();

        public:
            /// Configure the controller
            void Configure( const scarab::param_node& node );

            MEMVAR( unsigned, CycleTimeMS );

        public:   
            /// Use this to make a thread wait on a break
            /// If the return is true, processing should continue after the break
            /// If the return is false, processing has ended (either normally or due to an error) 
            virtual bool WaitToContinue();

            /// Use this to wait for a breakpoint to be reached or for cancellation
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

            /// Inititate a break
            virtual void Break();

            /// Initiate a break with a return
            template< typename... XArgs >
            void BreakAndReturn( XArgs&... args );

            /// Reports whether control is at a breakpoint
            virtual bool IsAtBreak() const;

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            MEMVAR_REF_MUTABLE( std::mutex, Mutex );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarContinue );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarBreak );
            MEMVAR_NOSET( bool, BreakFlag );

        protected:
            void do_cancellation( int code );

        public:
            /// Checks whether the return buffer has been filled
            bool HasReturn() const;

            /// Get the return buffer
            template< typename... XArgs >
            std::tuple< XArgs&... >& GetReturn();

        protected:
            std::unique_ptr< ReturnBufferBase > fReturnBuffer;
            MEMVAR_REF_MUTABLE( std::mutex, ReturnMutex );

    };

    inline bool Controller::HasReturn() const
    {
        return fReturnBuffer.operator bool();
    }

    template< typename... XArgs >
    void Controller::BreakAndReturn( XArgs&... args )
    {
        this->Break();
        fReturnBuffer.reset( new ReturnBuffer( args... ) );
        return;
    }

    template< class... XArgs >
    std::tuple< XArgs&... >& Controller::GetReturn()
    {
        std::unique_lock< std::mutex > lock( fReturnMutex );
        return fReturnBuffer->GetReturn< XArgs... >();
    }

} /* namespace Nymph */

#endif /* NYMPH_CONTROLLER_HH_ */
