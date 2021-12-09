/*
 * KTSignalSlotBase.hh
 *
 *  Created on: Feb 25, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTSIGNALSLOTBASE_HH_
#define NYMPH_KTSIGNALSLOTBASE_HH_

#include "KTMemberVariable.hh"

#include <string>

namespace Nymph
{
    class KTSignalBase;

    class KTSlotBase
    {
        public:
            KTSlotBase( const std::string& name );
            template< typename XOwner >
            KTSlotBase( const std::string& name, XOwner* owner );
            virtual ~KTSlotBase();

            virtual void AddConnection( KTSignalBase* ) const = 0;

            virtual void RemoveConnection( KTSignalBase* ) const = 0;

            MEMBERVARIABLE_REF( std::string, Name );
    };


    class KTSignalBase
    {
        public:
            KTSignalBase( const std::string& name );
            template< typename x_owner >
            KTSignalBase( const std::string& name, x_owner* owner );
            virtual ~KTSignalBase();

            virtual void Connect( KTSlotBase* slot, int group ) = 0;

            virtual void Disconnect( KTSlotBase* slot ) const = 0;

            MEMBERVARIABLE_REF( std::string, Name );
    };

} /* namespace Nymph */

#endif /* NYMPH_KTSIGNALSLOTBASE_HH_ */