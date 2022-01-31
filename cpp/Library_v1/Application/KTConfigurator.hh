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
<<<<<<< HEAD:Library_v1/Application/KTConfigurator.hh
        try
        {
            fParamBuffer = &const_cast< scarab::param& >( (*fMasterConfig)[aName] );
            if( fParamBuffer->is_value() )
            {
                return (*fParamBuffer)().as< XReturnType >();
            }
        }
        catch( std::exception& e )
        {}
        BOOST_THROW_EXCEPTION( KTException() << "configurator does not have a value for <" << aName << ">" << eom );
=======
        return fMasterConfig[ aName ]().as< XReturnType >();
>>>>>>> develop:Library/Application/KTConfigurator.hh
    }

    template< typename XReturnType >
    XReturnType KTConfigurator::Get( const std::string& aName, XReturnType aDefault ) const
    {
<<<<<<< HEAD:Library_v1/Application/KTConfigurator.hh
        try
        {
            fParamBuffer = &const_cast< scarab::param& >( (*fMasterConfig)[aName] );
            if( fParamBuffer->is_value() )
            {
                return (*fParamBuffer)().as< XReturnType >();
            }
        }
        catch( std::exception& e )
        {}
        return aDefault;
    }

    inline void KTConfigurator::Merge(const scarab::param_node& aNode)
    {
        fMasterConfig->merge(aNode);
        return;
    }

    inline scarab::param_node& KTConfigurator::Config()
    {
        return *fMasterConfig;
=======
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
>>>>>>> develop:Library/Application/KTConfigurator.hh
    }

    inline const scarab::param_node& KTConfigurator::Config() const
    {
        return *fMasterConfig;
    }



} /* namespace Nymph */
#endif /* KTCONFIGURATOR_HH_ */
