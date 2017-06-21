/*
 * KTCutResult.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef KTCUTRESULT_HH_
#define KTCUTRESULT_HH_

#include "KTExtensibleStruct.hh"

#include "KTMemberVariable.hh"

#include <boost/serialization/serialization.hpp>

#include <string>

namespace bs = boost::serialization;

namespace Nymph
{
    class KTCutResultCore
    {
        public:
            KTCutResultCore();
            virtual ~KTCutResultCore();

            virtual const std::string& Name() const = 0;

            MEMBERVARIABLE(bool, State);

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version );
    };

    typedef KTExtensibleStructCore< KTCutResultCore > KTCutResult;

    template< class XDerivedType >
    class KTExtensibleCutResult : public KTExtensibleStruct< XDerivedType, KTCutResultCore >
    {
        public:
            KTExtensibleCutResult() {}
            virtual ~KTExtensibleCutResult() {}

            const std::string& Name() const;

        private:
            friend class bs::access;

            template< class Archive >
            void serialize( Archive& ar, const unsigned version );
    };

    template< class Archive >
    void KTCutResultCore::serialize( Archive& ar, const unsigned version )
    {
        std::cout << "### serialize for KTCutResultCore" << std::endl;
        ar & fState;
        return;
    }

    template< class XDerivedType >
    template< class Archive >
    void KTExtensibleCutResult< XDerivedType >::serialize( Archive& ar, const unsigned version )
    {
        std::cout << "### serialize for KTExtensibleCutResult< XDerivedType >" << std::endl;
        ar & bs::base_object< KTExtensibleStruct< XDerivedType, KTCutResultCore > >( *this );
        return;
    }

    template< class XDerivedType >
    inline const std::string& KTExtensibleCutResult< XDerivedType >::Name() const
    {
        return XDerivedType::sName;
    }

} /* namespace Nymph */

#endif /* KTCUTRESULT_HH_ */
