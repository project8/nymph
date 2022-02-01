/*
 * RunNymph.cc
 *
 *  Created on: Sep 13, 2016
 *      Author: obla999
 */

#include "KTApplication.hh"
#include "KTLogger.hh"
#include "KTProcessorToolbox.hh"

#include "param.hh"

#include <string>

KTLOGGER( nlog, "RunNymph" );

namespace Nymph
{
    int RunNymph( std::shared_ptr< KTApplication > app )
    {
        try
        {
            const scarab::param_node& parentConfigNode = app->GetConfigurator()->Config();

            if( ! app->Configure( parentConfigNode[app->GetConfigName()].as_node() ) )
            {
                KTERROR( nlog, "Unable to configure the application. Aborting." );
                return -2;
            }

            // Create and configure the processor toolbox.
            // This will create all of the requested processors, connect their signals and slots, and fill the run queue.
            KTProcessorToolbox procTB;

<<<<<<< HEAD:Library_v1/Application/KTRunNymph.cc
            if ( ! procTB.Configure( parentConfigNode[procTB.GetConfigName()].as_node() ) )
=======
            if( ! procTB.Configure( parentConfigNode[procTB.GetConfigName()].as_node() ) )
>>>>>>> develop:Library/Application/KTRunNymph.cc
            {
                KTERROR( nlog, "Unable to configure processor toolbox. Aborting." );
                return -3;
            }

            // Configure the processors
            if( ! procTB.ConfigureProcessors( parentConfigNode ) )
            {
                KTERROR( nlog, "Unable to configure processors. Aborting." );
                return -4;
            }

            // Execute the run queue!
            bool success = procTB.Run();

            procTB.ClearProcessors();

            KTPROG( nlog, "That's all, folks!" );

            if( ! success ) return -5;
            return 0;
        }
        catch( std::exception& e )
        {
            KTERROR(  nlog, "Exception caught:\n" << e.what() );
            return -1;
        }
    }
}
