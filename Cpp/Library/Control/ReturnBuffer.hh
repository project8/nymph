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

    /*!
     @class ReturnBufferBase
     @author N. S. Oblath

     @brief Base class for ReturnBuffers, providing access to the returned parameters

     @details
     ReturnBuffers are a type-erasure mechanism for returning parameters at a breakpoint in Nymph operations.

     This base class provides the type erasure while grating access to the returned paramters via a templated GetREturn() function.
    */
    class ReturnBufferBase
    {
        public:
            ReturnBufferBase();
            virtual ~ReturnBufferBase();

            /// Provides access to the returned parameters
            /// Throws Nymph::Exception if the buffer is empty or if XArgs... does not match the stored contents
            template< class... XArgs >
            std::tuple< XArgs&... >& GetReturn();

            /// Provides const access to the returned parameters
            /// Throws Nymph::Exception if the buffer is empty or if XArgs... does not match the stored contents
            template< class... XArgs >
            const std::tuple< XArgs&... >& GetReturn() const;
    };

    /*!
     @class ReturnBuffer<XArgs...>
     @author N. S. Oblath

     @brief Provides storage for returned parameters

     @details
     Parameters are stored as a `std::tuple<XArgs...>`.
    */
    template< class... XArgs >
    class ReturnBuffer : public ReturnBufferBase
    {
        public:
            ReturnBuffer() = delete;
            ReturnBuffer( XArgs&... retval );
            virtual ~ReturnBuffer();

            /// Provides access to the returned parameters
            /// Throws Nymph::Exception if the buffer is empty
            std::tuple< XArgs&... >& GetReturn();

            /// Provides const access to the returned parameters
            /// Throws Nymph::Exception if the buffer is empty
            const std::tuple< XArgs&... >& GetReturn() const;

        protected:
            std::tuple< XArgs&... > fReturn;
    };


    // ReturnBuffer

    template< class... XArgs >
    ReturnBuffer< XArgs... >::ReturnBuffer( XArgs&... retval ) :
            fReturn( retval... )
    {}

    template< class... XArgs >
    ReturnBuffer< XArgs... >::~ReturnBuffer()
    {}

    template< class... XArgs >
    std::tuple< XArgs&... >& ReturnBuffer< XArgs... >::GetReturn()
    {
        return fReturn;
    }

    template< class... XArgs >
    const std::tuple< XArgs&... >& ReturnBuffer< XArgs... >::GetReturn() const
    {
        return fReturn;
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
