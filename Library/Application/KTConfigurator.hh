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

            scarab::param_node* Config();
            const scarab::param_node* Config() const;

            template< typename XReturnType >
            XReturnType Get( const std::string& aName ) const;

            template< typename XReturnType >
            XReturnType Get( const std::string& aName, XReturnType aDefault ) const;

        private:
            scarab::param_node* fMasterConfig;

            mutable scarab::param* fParamBuffer;

            std::string fStringBuffer;
    };

    template< typename XReturnType >
    XReturnType KTConfigurator::Get( const std::string& aName ) const
    {
        fParamBuffer = const_cast< scarab::param* >( fMasterConfig->at( aName ) );
        if( fParamBuffer != NULL && fParamBuffer->is_value() )
        {
            return fParamBuffer->as_value().get< XReturnType >();
        }
        throw KTException() << "configurator does not have a value for <" << aName << ">";
    }

    template< typename XReturnType >
    XReturnType KTConfigurator::Get( const std::string& aName, XReturnType aDefault ) const
    {
        fParamBuffer = const_cast< scarab::param* >( fMasterConfig->at( aName ) );
        if( fParamBuffer != NULL && fParamBuffer->is_value() )
        {
            return fParamBuffer->as_value().get< XReturnType >();
        }
        return aDefault;

    }


} /* namespace Nymph */
#endif /* KTCONFIGURATOR_HH_ */
