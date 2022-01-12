/*
 * KTConfigurator.hh
 *
 *  Created on: Nov 5, 2013
 *      Author: nsoblath
 */

#ifndef KTCONFIGURATOR_HH_
#define KTCONFIGURATOR_HH_

#include "singleton.hh"
#include "param.hh"

#include "KTException.hh"

#include <string>

namespace Nymph
{

    class KTConfigurator : public scarab::singleton< KTConfigurator >
    {
        private:
            friend class scarab::singleton< KTConfigurator >;
            friend class scarab::destroyer< KTConfigurator >;

            KTConfigurator();
            virtual ~KTConfigurator();

        public:
            void Merge(const scarab::param_node& aNode);

            scarab::param_node& Config();
            const scarab::param_node& Config() const;

            template< typename XReturnType >
            XReturnType Get( const std::string& aName ) const;

            template< typename XReturnType >
            XReturnType Get( const std::string& aName, XReturnType aDefault ) const;

        private:
            scarab::param_node fMasterConfig;

            std::string fStringBuffer;
    };

    template< typename XReturnType >
    XReturnType KTConfigurator::Get( const std::string& aName ) const
    {
        return fMasterConfig[ aName ]().as< XReturnType >();
    }

    template< typename XReturnType >
    XReturnType KTConfigurator::Get( const std::string& aName, XReturnType aDefault ) const
    {
        return fMasterConfig.get_value< XReturnType >( aName, aDefault );
    }

    inline void KTConfigurator::Merge(const scarab::param_node& aNode)
    {
        fMasterConfig.merge(aNode);
        return;
    }

    inline scarab::param_node& KTConfigurator::Config()
    {
        return fMasterConfig;
    }

    inline const scarab::param_node& KTConfigurator::Config() const
    {
        return fMasterConfig;
    }


} /* namespace Nymph */
#endif /* KTCONFIGURATOR_HH_ */
