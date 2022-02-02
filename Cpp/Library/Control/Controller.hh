/*
 * Controller.hh
 *
 *  Created on: Jan 13, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLLER_HH_
#define NYMPH_CONTROLLER_HH_

#include "ControllerInterface.hh"

#include "MemberVariable.hh"

#include "cancelable.hh"
#include "param.hh"

#include <condition_variable>
#include <mutex>

namespace Nymph
{

    /*!
     @class Controller
     @author N. S. Oblath

     @brief Base class for Controller classes, with basic break/continue/cancel functionality

    */
    class Controller : public ControllerInterface, public scarab::cancelable
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

            /// Reports whether control is at a breakpoint
            virtual bool IsAtBreak() const;

            /// Notify the control that a chain is quitting
            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() );

            MEMVAR_REF_MUTABLE( std::mutex, Mutex );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarContinue );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarBreak );
            MEMVAR_NOSET( bool, BreakFlag );
            //MEMVAR_SHARED_PTR_CONST( ReturnBufferBase, ReturnPtr );

        protected:
            void do_cancellation( int code );

    };

/*
    template< typename... Args >
    void SharedControl::Break( Args&... args )
    {
        while( IsAtBreak() && ! IsCanceled() )
        {
            if( ! WaitToContinue() )
            {
                THROW_EXCEPT_HERE( Exception() << "Canceled while waiting to initiate a breakpoint" );
            }
        }

        std::unique_lock< std::mutex > lock( fMutex );
        fBreakFlag = true;
        fCondVarBreak.notify_all();
        return;
    }



    template< typename... Args >
    std::tuple< Args&... >& SharedControl::GetReturn()
    {
        if( ! fReturnPtr ) THROW_EXCEPT_HERE( Exception() << "No return available" );
        std::unique_lock< std::mutex > lock( fMutex );
        std::shared_ptr< ReturnBuffer< Args... > > buffer( std::dynamic_pointer_cast< ReturnBuffer< Args... > >(fReturnPtr) );
        if( buffer == nullptr ) THROW_EXCEPT_HERE( Exception() << "Incorrect types used to get return" );
        return buffer->GetReturn();
    }
*/

} /* namespace Nymph */

#endif /* NYMPH_CONTROLLER_HH_ */
