/*
 * ControlAccess.hh
 *
 *  Created on: Sept 14, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLACCESS_HH_
#define NYMPH_CONTROLACCESS_HH_

#include "MemberVariable.hh"

#include "cancelable.hh"

//#include <boost/thread/condition_variable.hpp>
//#include <boost/thread/mutex.hpp>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <tuple>

namespace Nymph
{
    class SharedControl
    {
        public:
            SharedControl();
            virtual ~SharedControl();

            void Break(); // to be called by a working thread

            void Cancel(); // to be called by a working thread

            bool IsAtBreak() const; // to be called by a working thread

            bool IsCanceled() const; // to be called by a working thread

            // return: true = continue; false = cancel
            bool WaitToContinue() const; // to be called by a working thread

            void Resume(); // to be called by the processor toolbox

            MEMVAR_REF( std::mutex, Mutex );
            MEMVAR_REF( std::condition_variable, CondVar );
            MEMVAR_NOSET( bool, BreakFlag );
            MEMVAR_NOSET( bool, CanceledFlag );
            MEMVAR( unsigned, CycleTimeMS );

    }

    class ReturnBufferBase
    {
        public:
            ReturnBufferBase();
            virtual ~ReturnBufferBase();
    };

    template< typename... Args >
    struct ReturnBuffer : ReturnBufferBase
    {
        public:
            ReturnBuffer() :
                    fReturn( nullptr )
            {}

            ReturnBuffer( Args&... retval ) :
                    fReturn( new std::tuple< Args&... >( retval... ) )
            {}

            virtual ~ReturnBuffer()
            {}

            std::tuple< Args&... >& GetReturn()
            {
                if( fReturn == nullptr ) throw std::exception();
                return *fReturn;
            }

        protected:
            std::tuple< Args&... >* fReturn;
    };

    template<>
    struct ReturnBuffer< void > : ReturnBufferBase
    {
        ReturnBuffer() :
                ReturnBufferBase()
        {}

        void GetReturn()
        {
            return;
        }
    };

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
            MEM_VAR_SHARED_PTR( SharedControl, Control );
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
