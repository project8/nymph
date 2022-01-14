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
