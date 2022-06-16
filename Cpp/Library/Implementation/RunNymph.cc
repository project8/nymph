/*
 * RunNymph.cc
 *
 *  Created on: Sep 13, 2016
 *      Author: obla999
 */

#include "ProcessorToolbox.hh"
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

            // Create and configure the processor toolbox.
            ProcessorToolbox procTB;
            procTB.Configure( config);

            // Create and configure the single-run controller.
            LPROG( nlog, "Configuring controller" );
            SingleRunController controller( procTB );
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
}
