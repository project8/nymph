/*
 * RunNymph.cc
 *
 *  Created on: Sep 13, 2016
 *      Author: obla999
 */

#include "ProcessorToolbox.hh"
#include "SingleRunController.hh"

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

            LPROG( nlog, "Executing run" );

            // Now execute the run
            controller.Run();

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
}
