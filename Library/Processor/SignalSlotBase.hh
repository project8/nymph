/*
 * SignalSlotBase.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_SIGNALSLOTBASE_HH_
#define NYMPH_SIGNALSLOTBASE_HH_

#include "MemberVariable.hh"

#include <memory>
#include <string>

namespace Nymph
{
    class SignalBase;
    class SlotBase;

    typedef std::shared_ptr< SignalBase > SignalPtr_t;
    typedef std::shared_ptr< SlotBase > SlotPtr_t;

    class SlotBase
    {
        public:
            SlotBase( const std::string& name );
            //template< typename XOwner >
            //SlotBase( const std::string& name, XOwner* owner );
            virtual ~SlotBase();

            virtual void AddConnection( SignalPtr_t ) const = 0;

            virtual void RemoveConnection( SignalPtr_t ) const = 0;

            MEMVAR_REF( std::string, Name );
    };


    class SignalBase
    {
        public:
            SignalBase( const std::string& name );
            //template< typename XOwner >
            //SignalBase( const std::string& name, XOwner* owner );
            virtual ~SignalBase();

            virtual void Connect( SlotPtr_t slot, int group ) = 0;

            virtual void Disconnect( SlotPtr_t slot ) const = 0;

            MEMVAR_REF( std::string, Name );
    };

} /* namespace Nymph */

#endif /* NYMPH_SIGNALSLOTBASE_HH_ */
