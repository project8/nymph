/*
 * ReturnBuffer.hh
 *
 *  Created on: Oct 22, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_RETURNBUFFER_HH_
#define NYMPH_RETURNBUFFER_HH_

#include "Exception.hh"

#include <tuple>

namespace Nymph
{

    class ReturnBufferBase
    {
        public:
            ReturnBufferBase();
            virtual ~ReturnBufferBase();

            template< class... XArgs >
            std::tuple< XArgs&... >& GetReturn();

            template< class... XArgs >
            const std::tuple< XArgs&... >& GetReturn() const;
    };

    template< class... XArgs >
    class ReturnBuffer : public ReturnBufferBase
    {
        public:
            ReturnBuffer();
            ReturnBuffer( XArgs&... retval );
            virtual ~ReturnBuffer();

            std::tuple< XArgs&... >& GetReturn();

            const std::tuple< XArgs&... >& GetReturn() const;

        protected:
            std::tuple< XArgs&... >* fReturn;
    };


    // ReturnBuffer
    template< class... XArgs >
    ReturnBuffer< XArgs... >::ReturnBuffer() :
            fReturn( nullptr )
    {}

    template< class... XArgs >
    ReturnBuffer< XArgs... >::ReturnBuffer( XArgs&... retval ) :
            fReturn( new std::tuple< XArgs&... >( retval... ) )
    {}

    template< class... XArgs >
    ReturnBuffer< XArgs... >::~ReturnBuffer()
    {}

    template< class... XArgs >
    std::tuple< XArgs&... >& ReturnBuffer< XArgs... >::GetReturn()
    {
        if( fReturn == nullptr ) THROW_EXCEPT_HERE( Exception() << "No return is present" );
        return *fReturn;
    }

    template< class... XArgs >
    const std::tuple< XArgs&... >& ReturnBuffer< XArgs... >::GetReturn() const
    {
        if( fReturn == nullptr ) THROW_EXCEPT_HERE( Exception() << "No return is present" );
        return *fReturn;
    }


    // Templated functions from ReturnBufferBase
    template< class... XArgs >
    std::tuple< XArgs&... >& ReturnBufferBase::GetReturn()
    {
        ReturnBuffer< XArgs... >* typedBuffer = dynamic_cast< ReturnBuffer< XArgs... >* >(this);
        if( ! typedBuffer ) THROW_EXCEPT_HERE( Exception() << "Invalid set of arguments for this return buffer" );
        return typedBuffer->GetReturn();
    }

    template< class... XArgs >
    const std::tuple< XArgs&... >& ReturnBufferBase::GetReturn() const
    {
        ReturnBuffer< XArgs... >* typedBuffer = dynamic_cast< ReturnBuffer< XArgs... >* >(this);
        if( ! typedBuffer ) THROW_EXCEPT_HERE( Exception() << "Invalid set of arguments for this return buffer" );
        return typedBuffer->GetReturn();
    }


} /* namespace Nymph */

#endif /* NYMPH_RETURNBUFFER_HH_ */
