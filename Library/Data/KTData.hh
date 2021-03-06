/*
 * KTData.hh
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#ifndef KTDATA_HH_
#define KTDATA_HH_

#include "KTExtensibleStruct.hh"

#include "KTCutStatus.hh"
#include "KTMemberVariable.hh"

#include <boost/shared_ptr.hpp>

#include <string>

namespace Nymph
{
    class KTDataCore
    {
        public:
            KTDataCore() {}
            virtual ~KTDataCore() {}

            virtual const std::string& Name() const = 0;

    };

    template< class XDerivedType >
    class KTExtensibleData : public KTExtensibleStruct< XDerivedType, KTDataCore >
    {
        public:
            KTExtensibleData() {}
            virtual ~KTExtensibleData() {}

            const std::string& Name() const;

    };

    template< class XDerivedType >
    inline const std::string& KTExtensibleData< XDerivedType >::Name() const
    {
        return XDerivedType::sName;
    }



    class KTData : public KTExtensibleData< KTData >
    {
        public:
            KTData();
            KTData(const KTData& orig);
            ~KTData();

            MEMBERVARIABLE(unsigned, Counter);
            MEMBERVARIABLE(bool, LastData);

            MEMBERVARIABLEREF_NOSET(KTCutStatus, CutStatus);

        public:
            static const std::string sName;
    };

    typedef boost::shared_ptr< KTData > KTDataPtr;

} /* namespace Nymph */
#endif /* KTDATA_HH_ */
