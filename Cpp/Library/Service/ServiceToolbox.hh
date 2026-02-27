/**
 @file ServiceToolbox.hh
 @brief Contains ServiceToolbox
 @details Manages svcessors requested by the user at run time.
 @author: N. S. Oblath
 @date: Dec 28, 2023
 */

#ifndef NYMPH_SERVICETOOLBOX_HH_
#define NYMPH_SERVICETOOLBOX_HH_

#include "factory.hh"
#include "param.hh"

#include <limits>
#include <set>
#include <memory>


namespace Nymph
{
    class Service;

    /*!
     @class ServiceToolbox
     @author N. S. Oblath

     @brief Manages services requested by the user at run time.

     @details
     ServiceToolbox allows the user to setup an application at runtime.

     The user chooses the services to be used via a configuration file.

     Available (nested) configuration values:
     <li>services (array of objects) -- create a service; each object in the array should consist of:
         <ul>
             <li>type -- string specifying the svcessor type (matches the string given to the Registrar, which should be specified before the class implementation in each service's .cc file).</li>
             <li>name -- string giving the individual svcessor a name so that multiple services of the same type can be created.</li>
         </ul>
     </li>
    */
    class ServiceToolbox
    {
        protected:
            typedef std::unique_lock< std::mutex > unique_lock;

        public:
            ServiceToolbox( const std::string& name = "service-toolbox" );
            virtual ~ServiceToolbox();

        protected:
            scarab::factory< Service, const std::string& >* fServiceFactory; // singleton; not owned by ServiceToolbox


        public:
            /// Configure the toolbox: create the services and configure; connnect signals and slots; and setup the run queue.
            void Configure( const scarab::param_node& node );

        protected:
            struct ServiceInfo
            {
                std::shared_ptr< Service > fService;
            };
            typedef std::map< std::string, ServiceInfo > ServiceMap;

        public:
            /// Create services and configure each according to the `services` configuration block
            void ConfigureServices( const scarab::param_array& node );

            /// Get a pointer to a service in the toolbox
            std::shared_ptr< Service > GetService( const std::string& svcName );
            /// Get a pointer to a service in the toolbox
            const std::shared_ptr< Service > GetService( const std::string& svcName ) const;

            /// Add a service to the toolbox
            /// Toolbox takes ownership of the service
            bool AddService( const std::string& svcName, std::shared_ptr< Service > svc );
            bool AddService( const std::string& svcType, const std::string& svcName );

            /// Check whether a service already has a serice with a given name
            bool HasService( const std::string& svcName ) const;
            /// Check whether a service could build a service of a given type (i.e. does the factory know about this service?)
            bool CouldBuild( const std::string& svcType ) const;

            /// Remove a service from the toolbox
            bool RemoveService( const std::string& svcName );

            /// Remove a service from the toolbox and return it to the user
            /// Ownership is passed to the user
            std::shared_ptr< Service > ReleaseService( const std::string& svcName );

            /// Remove all svcessors from the toolbox
            /// Also clears the run queue
            void ClearServices();

        protected:
            ServiceMap fServiceMap;

    };

    inline bool ServiceToolbox::HasService( const std::string& svcName ) const
    {
        return fServiceMap.count( svcName ) == 1;
    }

    inline bool ServiceToolbox::CouldBuild( const std::string& svcType ) const
    {
        return fServiceFactory->has_class( svcType );
    }


} /* namespace Nymph */
#endif /* NYMPH_SERVICETOOLBOX_HH_ */
