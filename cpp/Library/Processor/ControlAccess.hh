/*
 * ControlAccess.hh
 *
 *  Created on: Sept 14, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLACCESS_HH_
#define NYMPH_CONTROLACCESS_HH_

#include "Exception.hh"
#include "MemberVariable.hh"
#include "ReturnBuffer.hh"

#include "cancelable.hh" // remove when ControlAccess is removed
#include "singleton.hh"

//#include <boost/thread/condition_variable.hpp>
//#include <boost/thread/mutex.hpp>

#include <condition_variable>
#include <memory>
#include <mutex>

namespace Nymph
{

    class SharedControl : public scarab::singleton< SharedControl >
    {
        protected:
            allow_singleton_access( SharedControl );

            SharedControl();
            virtual ~SharedControl();

        public:
            template< typename... Args >
            std::tuple< Args&... >& GetReturn();

            template< typename... Args >
            void Break( Args&... args ); // to be called by a working thread

            void Cancel(); // to be called by a working thread

            bool IsAtBreak() const; // to be called by a working thread

            bool IsCanceled() const; // to be called by a working thread

            // return: true = continue; false = cancel
            bool WaitToContinue() const; // to be called by a working thread

            // return: true = was a break; false = cancel
            bool WaitForBreakOrCanceled() const; // to be called by the processor toolbox

            void Resume(); // to be called by the processor toolbox

            void IncrementActiveThreads();
            void DecrementActiveThreads();

            MEMVAR_REF_MUTABLE( std::mutex, Mutex );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarContinue );
            MEMVAR_REF_MUTABLE( std::condition_variable, CondVarBreak );
            MEMVAR_NOSET( bool, BreakFlag );
            MEMVAR_NOSET( bool, CanceledFlag );
            MEMVAR( unsigned, CycleTimeMS );
            MEMVAR_SHARED_PTR_CONST( ReturnBufferBase, ReturnPtr );
            MEMVAR( unsigned, NActiveThreads );

            // WARNING: This function resets the state of this class to a just-constructed state
            //          It is intended for use with testing when a single application might use it multiple times
            void Reset();

    };

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



    class ControlAccess : public scarab::cancelable
    {
        public:
            ControlAccess();

            virtual ~ControlAccess();

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
    void ControlAccess::SetReturn( Args&... args )
    {
        fReturn = std::make_shared< ReturnBuffer< Args... > >( args... );
        return;
    }

    template< typename... Args >
    std::tuple< Args&... >& ControlAccess::GetReturn()
    {
        std::shared_ptr< ReturnBuffer< Args... > > buffer( std::dynamic_pointer_cast< ReturnBuffer< Args... > >(fReturn) );
        if( buffer == nullptr ) throw std::exception();
        return buffer->GetReturn();
    }

} /* namespace Nymph */

#endif /* NYMPH_CONTROLACCESS_HH_ */