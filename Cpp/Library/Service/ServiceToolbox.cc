/*
 * ServiceToolbox.cc
 *
 *  Created on: Dec 28, 2023
 *      Author: N.S. Oblath
 */

#include "ServiceToolbox.hh"

#include "ConfigException.hh"
#include "Service.hh"

#include "factory.hh"
#include "logger.hh"
#include "param_codec.hh"

#include <map>

using std::string;

namespace Nymph
{
    LOGGER(servicelog, "ServiceToolbox");

    ServiceToolbox::ServiceToolbox( const std::string& name ) :
            fServiceFactory( scarab::factory< Service, const std::string& >::get_instance() ),
            fServiceMap()
    {
    }

    ServiceToolbox::~ServiceToolbox()
    {}

    void ServiceToolbox::Configure( const scarab::param_node& node )
    {
        LPROG( servicelog, "Configuring service toolbox" );

        // Deal with "service" blocks
        if( ! node.has("services") )
        {
            LWARN( servicelog, "No services were specified" );
        }
        else
        {
            ConfigureServices( node["services"].as_array() );
        }

        return;
    }


    void ServiceToolbox::ConfigureServices( const scarab::param_array& array )
    {
        for( auto serviceIt = array.begin(); serviceIt != array.end(); ++serviceIt )
        {
            if( ! serviceIt->is_node() )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Invalid service entry (not a node): " << *serviceIt );
            }
            const scarab::param_node& serviceNode = serviceIt->as_node();

            if( ! serviceNode.has("type") )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to create service: no service type given" );
            }
            string serviceType = serviceNode["type"]().as_string();

            string serviceName;
            if( ! serviceNode.has("name") )
            {
                LINFO(servicelog, "No name given for service of type <" << serviceType << ">; using type as name.");
                serviceName = serviceType;
            }
            else
            {
                serviceName = serviceNode["name"]().as_string();
            }

            std::shared_ptr< Service > newProc ( fServiceFactory->create(serviceType, serviceName) );
            if( newProc == nullptr )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to create service of type <" << serviceType << ">" );
            }

            LDEBUG( servicelog, "Attempting to configure service <" << serviceName << ">" );
            try
            {
                newProc->Configure(serviceNode);
            }
            catch( scarab::base_exception& e )
            {
                THROW_NESTED_EXCEPT_HERE( Exception() << "An error occurred while configuring service <" << serviceName << ">" );
            }

            if( ! AddService( serviceName, newProc ) )
            {
                THROW_EXCEPT_HERE( ConfigException(array) << "Unable to add service <" << serviceName << ">" );
            }
        }

        return;
    }

    std::shared_ptr< Service > ServiceToolbox::GetService( const std::string& serviceName )
    {
        if( auto it = fServiceMap.find( serviceName ); it != fServiceMap.end() )
        {
            return it->second.fService;
        }
        LWARN( servicelog, "Service <" << serviceName << "> was not found." );
        return nullptr;
    }

    const std::shared_ptr< Service > ServiceToolbox::GetService( const std::string& serviceName ) const
    {
        if( auto it = fServiceMap.find( serviceName ); it != fServiceMap.end() )
        {
            return it->second.fService;
        }
        LWARN( servicelog, "Service <" << serviceName << "> was not found." );
        return nullptr;
    }

    bool ServiceToolbox::AddService( const std::string& serviceName, std::shared_ptr< Service > service )
    {
        if( auto it = fServiceMap.find( serviceName); it == fServiceMap.end() )
        {
            ServiceInfo pInfo;
            pInfo.fService = service;
            fServiceMap.insert( ServiceMap::value_type(serviceName, pInfo) );
            LDEBUG( servicelog, "Added service <" << serviceName << "> (a.k.a. " << service->Name() << ")" );
            return true;
        }
        LWARN( servicelog, "Service <" << serviceName << "> already exists; new service was not added." );
        return false;
    }

    bool ServiceToolbox::AddService( const std::string& serviceType, const std::string& serviceName )
    {
        if( auto it = fServiceMap.find( serviceName ); it == fServiceMap.end() )
        {
            std::shared_ptr< Service > newProc ( fServiceFactory->create(serviceType, serviceType) );
            if( newProc == nullptr )
            {
                LERROR( servicelog, "Unable to create service of type <" << serviceType << ">" );
                return false;
            }
            if( ! AddService(serviceName, newProc) )
            {
                LERROR( servicelog, "Unable to add service <" << serviceName << ">" );
                return false;
            }
            return true;
        }
        LWARN( servicelog, "Service <" << serviceName << "> already exists; new service was not added." );
        return false;
    }

    bool ServiceToolbox::RemoveService( const std::string& serviceName )
    {
        if( auto serviceToRemove = ReleaseService( serviceName ); serviceToRemove != nullptr )
        {
            LDEBUG( servicelog, "Service <" << serviceName << "> deleted." );
            return true;
        }
        return false;
    }

    std::shared_ptr< Service > ServiceToolbox::ReleaseService( const std::string& serviceName )
    {
        if( auto it = fServiceMap.find( serviceName ); it != fServiceMap.end() )
        {
            std::shared_ptr< Service > serviceToRelease = it->second.fService;
            fServiceMap.erase( it );
            return serviceToRelease;
        }
        LWARN( servicelog, "Service <" << serviceName << "> was not found." );
        return nullptr;
    }

    void ServiceToolbox::ClearServices()
    {
        fServiceMap.clear();
        return;
    }

} /* namespace Nymph */
