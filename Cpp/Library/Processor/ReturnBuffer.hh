/*
 * ReturnBuffer.hh
 *
 *  Created on: Oct 22, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_RETURNBUFFER_HH_
#define NYMPH_RETURNBUFFER_HH_

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

} /* namespace Nymph */

#endif /* NYMPH_RETURNBUFFER_HH_ */
