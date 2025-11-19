/*
 * RunNymph.cc
 *
 *  Created on: Sep 13, 2016
 *      Author: obla999
 */

#include "ProcessorToolbox.hh"
#include "ServiceToolbox.hh"
#include "SingleRunController.hh"

#include "application.hh"
#include "logger.hh"
#include "param.hh"

#include <string>

LOGGER( nlog, "RunNymph" );

namespace Nymph
{
    int RunNymph( scarab::param_node& config )
    {
        try
        {
            LPROG( nlog, "Configuring processor toolbox" );

            // Create the toolboxes and inject dependency
            ProcessorToolbox procTB;
            ServiceToolbox svcTB;
            procTB.SetServiceToolbox( &svcTB );
            LERROR( nlog, "Service TB ptr: " << &svcTB << " -- " << procTB.GetServiceToolbox());

            // Configure toolboxes
            svcTB.Configure( config);
            procTB.Configure( config);

            // Create and configure the single-run controller.
            LPROG( nlog, "Configuring controller" );
            SingleRunController controller( procTB, svcTB );
            if( config.has("controller") )
            {
                controller.Configure( config["controller"].as_node() );
            }

            if( config.has( "dry-run" ) && config["dry-run"]().as_bool() )
            {
                LPROG( nlog, "Dry run: no execution" );
            }
            else
            {
                LPROG( nlog, "Executing run" );
                controller.Run();
            }

            LPROG( nlog, "That's all, folks!" );

            return RETURN_SUCCESS;
        }
        catch( scarab::base_exception& e )
        {
            LERROR( nlog, "Exception caught:" );
            PrintException( e );
            return RETURN_ERROR;
        }
        catch( std::exception& e )
        {
            LERROR(  nlog, "Exception caught:" );
            PrintException( e );
            return RETURN_ERROR;
        }
    }

    void AddRunNymphOptions( scarab::main_app& an_app )
    {
        // options
        an_app.add_config_flag< bool >( "--dry-run", "dry-run", "Load the config, setup processors, but do not execute the run" );
    }

    int ProcessorCheck( scarab::param_node& a_config )
    {
        ProcessorToolbox tb;
        
        LWARN( nlog, a_config );

        if( a_config.has( "list-procs" ) && a_config["list-procs"]().as_bool() )
        {
            using factory_type = const scarab::factory< Processor, const std::string& >;
            factory_type* t_factory = tb.ProcFactory();
            std::stringstream t_list_ss;
            for( auto it = t_factory->begin(); it != t_factory->end(); ++it )
            {
                t_list_ss << it->first << '\n';

            }
            LPROG( nlog, "Available processors:\n" << t_list_ss.str() );
            return RETURN_SUCCESS;
        }
        else
        {
            if( ! a_config.has( "proc-type" ) || a_config["proc-type"]().as_string().empty() )
            {
                LERROR( nlog, "No processor type was provided to check" );
                return RETURN_ERROR;
            }
            std::string t_proc_type( a_config["proc-type"]().as_string() );
            bool t_proc_is_available = tb.CouldBuild( t_proc_type );
            if( ! t_proc_is_available )
            {
                LWARN( nlog, "Processor <" << t_proc_type << "> is NOT registered with the processor toolbox" );
                return -1;
            }
            LPROG( nlog, "Processor <" << t_proc_type << "> is known to the processor toolbox" );
            return RETURN_SUCCESS;
        }
    }

    void AddProcessorCheckOptions( scarab::config_decorator* a_subcommand )
    {
        // options
        a_subcommand->add_config_option< std::string >( "proc-type", "proc-type", "Query Nymph to see if this processor type has been registered; returns 0 if present; -1 if not present" );
        a_subcommand->add_config_flag< bool >( "-l,--list-procs", "list-procs", "List available processors" );
    }
}
