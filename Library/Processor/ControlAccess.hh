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
            void Break(); // called by this thread
            void Resume(); // called by user thread

            MEMVAR_REF( std::mutex, Mutex );
            MEMVAR_REF( std::condition_variable, CondVar );
            MEMVAR_ATOMIC_NOSET( bool, BreakFlag );
            MEMVAR( unsigned, CycleTimeMS );

    };


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
