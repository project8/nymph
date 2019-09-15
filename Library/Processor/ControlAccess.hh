/*
 * ControlAccess.hh
 *
 *  Created on: Sept 14, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_CONTROLACCESS_HH_
#define NYMPH_CONTROLACCESS_HH_

#include <memory>

namespace Nymph
{
    class ReturnBufferBase
    {
        public:
            ReturnBufferBase();
            virtual ~ReturnBufferBase();
    };

    template< typename OneArg >
    struct ReturnBuffer : ReturnBufferBase
    {
        public:
            ReturnBuffer() :
                    fReturn( nullptr )
            {}

            ReturnBuffer( OneArg& retval ) :
                    fReturn( &retval )
            {}

            virtual ~ReturnBuffer()
            {}

            OneArg& GetReturn()
            {
                if( fReturn == nullptr ) throw std::exception();
                return *fReturn;
            }

        protected:
            OneArg* fReturn;
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

    //TODO fill this in with anything useful
    class ControlAccess
    {
        public:
            ControlAccess();

            virtual ~ControlAccess();

            template< typename OneArg >
            void SetReturn( OneArg& arg );

            template< typename OneArg >
            OneArg& GetReturn();

        protected:
            std::shared_ptr< ReturnBufferBase > fReturn;
    };


    template< typename OneArg >
    void ControlAccess::SetReturn( OneArg& arg )
    {
        fReturn = std::make_shared< ReturnBuffer< OneArg > >( arg );
        return;
    }

    template< typename OneArg >
    OneArg& ControlAccess::GetReturn()
    {
        std::shared_ptr< ReturnBuffer< OneArg > > buffer( std::dynamic_pointer_cast< ReturnBuffer< OneArg > >(fReturn) );
        if( buffer == nullptr ) throw std::exception();
        return buffer->GetReturn();
    }

} /* namespace Nymph */

#endif /* NYMPH_CONTROLACCESS_HH_ */
